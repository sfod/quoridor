#include "board.hpp"

#include <cstring>

#include <boost/lexical_cast.hpp>

#include "exception.hpp"


namespace Quoridor {

Board::Board(int size)
    : size_(size), pawn_goal_nodes_(), walls_(), occ_nodes_(), pawn_nodes_(),
    bg_(size_, size_)
{
    if ((size <= 0) || (size % 2 == 0)) {
        throw Exception("invalid board size: "
                + boost::lexical_cast<std::string>(size));
    }
}

Board::~Board()
{
}

int Board::add_pawn(std::shared_ptr<Pawn> pawn, const Node &start_node,
        const std::set<Node> &goal_nodes)
{
    occ_nodes_[start_node] = pawn;
    pawn_nodes_[pawn] = start_node;
    pawn_goal_nodes_[pawn] = goal_nodes;

    bg_.block_neighbours(start_node);

    return 0;
}

bool Board::is_occupied(const Node &node) const
{
    return occ_nodes_.find(node) != occ_nodes_.end();
}

Node Board::pawn_node(std::shared_ptr<Pawn> pawn) const
{
    return pawn_nodes_.at(pawn);
}

int Board::make_walking_move(std::shared_ptr<Pawn> pawn, const Node &node)
{
    Node cur_node = pawn_nodes_[pawn];

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
    Node node = pawn_nodes_.at(pawn);
    return pawn_goal_nodes_.at(pawn).count(node) != 0;
}

int Board::add_wall(const Wall &wall)
{
    std::vector<std::pair<Node, Node>> edges;
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
        std::vector<std::pair<Node, Node>> *edges)
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

    Node node1;
    Node node2;
    Node node_tmp;
    if (wall.orientation() == 0) {
        for (int i = 0; i < wall.cnt(); ++i) {
            node1 = Node(wall.line(), wall.start_pos() + i);
            node2 = Node(wall.line() + 1, wall.start_pos() + i);
            bg_.filter_edges(node1, node2);
            edges->push_back(std::make_pair(node1, node2));

            node_tmp = Node(wall.line() + 2, wall.start_pos() + i);
            if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                    && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                bg_.filter_edges(node1, node_tmp);
                edges->push_back(std::make_pair(node1, node_tmp));
            }

            node_tmp = Node(wall.line() - 1, wall.start_pos() + i);
            if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                    && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                bg_.filter_edges(node_tmp, node2);
                edges->push_back(std::make_pair(node_tmp, node2));
            }

            for (int j = i - 1; j <= i + 1; j += 2) {
                node_tmp = Node(wall.line() + 1, wall.start_pos() + j);
                if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                        && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                    bg_.filter_edges(node1, node_tmp);
                    edges->push_back(std::make_pair(node1, node_tmp));
                }

                node_tmp = Node(wall.line(), wall.start_pos() + j);
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
            node1 = Node(wall.start_pos() + i, wall.line());
            node2 = Node(wall.start_pos() + i, wall.line() + 1);
            bg_.filter_edges(node1, node2);
            edges->push_back(std::make_pair(node1, node2));

            node_tmp = Node(wall.start_pos() + i, wall.line() + 2);
            if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                    && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                bg_.filter_edges(node1, node_tmp);
                edges->push_back(std::make_pair(node1, node_tmp));
            }

            node_tmp = Node(wall.start_pos() + i, wall.line() - 1);
            if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                    && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                bg_.filter_edges(node_tmp, node2);
                edges->push_back(std::make_pair(node_tmp, node2));
            }

            for (int j = i - 1; j <= i + 1; j += 2) {
                node_tmp = Node(wall.start_pos() + j, wall.line() + 1);
                if ((node_tmp.row() >= 0) && (node_tmp.row() < size_)
                        && (node_tmp.col() >= 0) && (node_tmp.col() < size_)) {
                    bg_.filter_edges(node1, node_tmp);
                    edges->push_back(std::make_pair(node1, node_tmp));
                }

                node_tmp = Node(wall.start_pos() + j, wall.line());
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
        path_blocked = true;
        for (auto node : pawn_goal_nodes_[pawn_node.first]) {
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

bool Board::get_path(std::shared_ptr<Pawn> pawn, const Node &node,
        std::list<Node> *path) const
{
    return bg_.find_path(pawn_nodes_.at(pawn), node, path);
}

bool Board::is_possible_move(const Node &cur_node, const Node &node) const
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

}  /* namespace Quoridor */
