#include "board.hpp"

#include <cstring>

#include <boost/lexical_cast.hpp>

#include "exception.hpp"
#include "walk_move.hpp"


namespace Quoridor {

Board::Board(int size)
    : size_(size), sides_(), pawn_sides_(), walls_(), occ_nodes_(), pawn_nodes_(),
    bg_(size_, size_)
{
    if ((size <= 0) || (size % 2 == 0)) {
        throw Exception("invalid board size: "
                + boost::lexical_cast<std::string>(size));
    }

    sides_.push_back(std::pair<int, int>(0, 0));
    sides_.push_back(std::pair<int, int>(2, 0));
    sides_.push_back(std::pair<int, int>(1, 0));
    sides_.push_back(std::pair<int, int>(3, 0));
}

Board::~Board()
{
}

int Board::add_pawn(std::shared_ptr<Pawn> pawn)
{
    pawn_sides_[pawn] = next_side();
    Pos node;

    switch (pawn_sides_[pawn]) {
    case 0:
        node = Pos(0, size_ / 2);
        break;
    case 1:
        node = Pos(size_ / 2, 0);
        break;
    case 2:
        node = Pos(size_ - 1, size_ / 2);
        break;
    case 3:
        node = Pos(size_ / 2, size_ - 1);
        break;
    default:
        throw Exception("invalid pawn side: "
                + boost::lexical_cast<std::string>(pawn_sides_[pawn]));
    }

    occ_nodes_[node] = pawn;
    pawn_nodes_[pawn] = node;

    bg_.block_neighbours(node);

    return 0;
}

bool Board::is_occupied(const Pos &node) const
{
    return occ_nodes_.find(node) != occ_nodes_.end();
}

Pos Board::pawn_node(std::shared_ptr<Pawn> pawn) const
{
    return pawn_nodes_.at(pawn);
}

int Board::make_walking_move(std::shared_ptr<Pawn> pawn, const Pos &node)
{
    Pos cur_node = pawn_nodes_[pawn];

    if (!is_possible_move(cur_node, node)) {
        return -1;
    }

    // update pawn's position
    occ_nodes_.erase(pawn_nodes_[pawn]);
    bg_.unblock_neighbours(pawn_nodes_[pawn]);
    pawn_nodes_[pawn] = node;
    occ_nodes_[node] = pawn;
    bg_.block_neighbours(node);

    return 0;
}

bool Board::is_at_goal_node(std::shared_ptr<Pawn> pawn) const
{
    Pos node = pawn_nodes_.at(pawn);
    switch (pawn_sides_.at(pawn)) {
    case 0:
        return node.row() == size_ - 1;
    case 1:
        return node.col() == size_ - 1;
    case 2:
        return node.row() == 0;
    case 3:
        return node.col() == 0;
    default:
        throw Exception("invalid board side: "
                + boost::lexical_cast<std::string>(pawn_sides_.at(pawn)));
    }
}

int Board::add_wall(const Wall &wall)
{
    std::vector<std::pair<Pos, Pos>> edges;
    if (try_add_wall(wall, &edges) < 0) {
        return -1;
    }

    walls_[wall.orientation()][wall.line()].insert(std::map<int, Wall>::value_type(wall.start_pos(), Wall(wall)));

    for (auto edge : edges) {
        bg_.remove_edges(edge.first, edge.second);
    }


    return 0;
}

int Board::try_add_wall(const Wall &wall,
        std::vector<std::pair<Pos, Pos>> *edges)
{
    int line_lim = size_ - 1;
    int start_pos_lim = size_ - 1;
    if ((wall.line() >= line_lim)
            || (wall.end_pos() > start_pos_lim)) {
        return -1;
    }

    if (wall_intersects(wall)) {
        return -1;
    }

    bg_.reset_filters();

    Pos node1;
    Pos node2;
    Pos node_tmp;
    if (wall.orientation() == 0) {
        for (int i = 0; i < wall.cnt(); ++i) {
            node1 = Pos(wall.line(), wall.start_pos() + i);
            node2 = Pos(wall.line() + 1, wall.start_pos() + i);
            bg_.filter_edges(node1, node2);
            edges->push_back(std::make_pair(node1, node2));

            node_tmp = Pos(wall.line() + 2, wall.start_pos() + i);
            if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                    && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                bg_.filter_edges(node1, node_tmp);
                edges->push_back(std::make_pair(node1, node_tmp));
            }

            node_tmp = Pos(wall.line() - 1, wall.start_pos() + i);
            if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                    && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                bg_.filter_edges(node_tmp, node2);
                edges->push_back(std::make_pair(node_tmp, node2));
            }

            for (int j = i - 1; j <= i + 1; j += 2) {
                node_tmp = Pos(wall.line() + 1, wall.start_pos() + j);
                if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                        && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                    bg_.filter_edges(node1, node_tmp);
                    edges->push_back(std::make_pair(node1, node_tmp));
                }

                node_tmp = Pos(wall.line(), wall.start_pos() + j);
                if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                        && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                    bg_.filter_edges(node_tmp, node2);
                    edges->push_back(std::make_pair(node_tmp, node2));
                }

            }
        }
    }
    else {
        for (int i = 0; i < wall.cnt(); ++i) {
            node1 = Pos(wall.start_pos() + i, wall.line());
            node2 = Pos(wall.start_pos() + i, wall.line() + 1);
            bg_.filter_edges(node1, node2);
            edges->push_back(std::make_pair(node1, node2));

            node_tmp = Pos(wall.start_pos() + i, wall.line() + 2);
            if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                    && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                bg_.filter_edges(node1, node_tmp);
                edges->push_back(std::make_pair(node1, node_tmp));
            }

            node_tmp = Pos(wall.start_pos() + i, wall.line() - 1);
            if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                    && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                bg_.filter_edges(node_tmp, node2);
                edges->push_back(std::make_pair(node_tmp, node2));
            }

            for (int j = i - 1; j <= i + 1; j += 2) {
                node_tmp = Pos(wall.start_pos() + j, wall.line() + 1);
                if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                        && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                    bg_.filter_edges(node1, node_tmp);
                    edges->push_back(std::make_pair(node1, node_tmp));
                }

                node_tmp = Pos(wall.start_pos() + j, wall.line());
                if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                        && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                    bg_.filter_edges(node_tmp, node2);
                    edges->push_back(std::make_pair(node_tmp, node2));
                }
            }
        }
    }

    bool path_blocked = false;

    for (auto pawn_node : pawn_nodes_) {
        std::vector<Pos> nodes;
        int side = (pawn_sides_[pawn_node.first] + 2) % 4;
        path_blocked = true;

        side_nodes(side, &nodes);
        for (auto node : nodes) {
            if (bg_.is_path_exists(pawn_node.second, node)) {
                path_blocked = false;
                break;
            }
        }

        // wall blocks all pathes to the opposite side for one of pawns
        if (path_blocked) {
            break;
        }
    }

    if (path_blocked) {
        return -1;
    }

    return 0;
}

void Board::pawn_goal_nodes(std::shared_ptr<Pawn> pawn,
        std::vector<Pos> *nodes) const
{
    int side = (pawn_sides_.at(pawn) + 2) % 4;
    side_nodes(side, nodes);
}

bool Board::get_path(std::shared_ptr<Pawn> pawn, const Pos &node,
        std::list<Pos> *path) const
{
    return bg_.find_path(pawn_nodes_.at(pawn), node, path);
}

int Board::next_side() const
{
    for (auto &side : sides_) {
        if (side.second == 0) {
            side.second = 1;
            return side.first;
        }
    }
    return -1;
}

bool Board::is_possible_move(const Pos &cur_node, const Pos &node) const
{
    return bg_.is_adjacent(cur_node, node);
}

bool Board::wall_intersects(const Wall &wall) const
{
    // check intersections
    if (walls_.count(1 - wall.orientation()) > 0) {
        for (int i = 0; i < wall.cnt() - 1; ++i) {
            // there are walls on the intersected line
            if (walls_.at(1 - wall.orientation()).count(wall.start_pos()) != 0) {
                auto line_walls = walls_.at(1 - wall.orientation()).at(wall.start_pos() + i);
                auto it = line_walls.upper_bound(wall.line());

                // all walls on the line are settled before new wall
                if (it == line_walls.end()) {
                    auto rit = line_walls.rbegin();
                    if (rit->second.end_pos() >= wall.line()) {
                        return true;
                    }
                }
                else if (it != line_walls.begin()) {
                    --it;
                    if (it->second.end_pos() >= wall.line()) {
                        return true;
                    }
                }
            }
        }
    }

    // check overlaps
    if (walls_.count(wall.orientation()) > 0) {
        if (walls_.at(wall.orientation()).count(wall.line()) != 0) {
            auto line_walls = walls_.at(wall.orientation()).at(wall.line());
            auto it = line_walls.upper_bound(wall.start_pos());

            // all walls on the line are settled before new wall
            if (it == line_walls.end()) {
                auto rit = line_walls.rbegin();
                if (rit->second.end_pos() >= wall.start_pos()) {
                    return true;
                }
            }
            else {
                // check if new wall overlaps over next wall on the line
                if (wall.end_pos() >= it->second.start_pos()) {
                    return true;
                }

                if (it != line_walls.begin()) {
                    --it;
                    if (it->second.end_pos() >= wall.start_pos()) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void Board::side_nodes(int side, std::vector<Pos> *nodes) const
{
    switch (side) {
    case 0:
        for (int i = 0; i < size_; ++i) {
            nodes->push_back(Pos(0, i));
        }
        break;
    case 1:
        for (int i = 0; i < size_; ++i) {
            nodes->push_back(Pos(i, 0));
        }
        break;
    case 2:
        for (int i = 0; i < size_; ++i) {
            nodes->push_back(Pos(size_ - 1, i));
        }
        break;
    case 3:
        for (int i = 0; i < size_; ++i) {
            nodes->push_back(Pos(i, size_ - 1));
        }
        break;
    default:
        break;
    }
}

}  /* namespace Quoridor */
