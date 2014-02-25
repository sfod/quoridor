#include "board.hpp"

#include <cstring>

#include <boost/lexical_cast.hpp>

#include "exception.hpp"
#include "walk_move.hpp"


namespace Quoridor {

Board::Board(int row_num, int col_num)
    : sides_(), pawn_sides_(), walls_(), occ_nodes_(), pawn_nodes_(), bg_()
{
    set_size(row_num, col_num);

    sides_.push_back(std::pair<int, int>(0, 0));
    sides_.push_back(std::pair<int, int>(2, 0));
    sides_.push_back(std::pair<int, int>(1, 0));
    sides_.push_back(std::pair<int, int>(3, 0));
}

Board::~Board()
{
}

void Board::set_size(int row_num, int col_num)
{
    if (row_num <= 0) {
        throw Exception("illegal row number: "
                + boost::lexical_cast<std::string>(row_num));
    }
    if (col_num <= 0) {
        throw Exception("illegal column number: "
                + boost::lexical_cast<std::string>(col_num));
    }
    if (row_num % 2 == 0) {
        throw Exception("row number must be odd: "
                + boost::lexical_cast<std::string>(row_num));
    }
    if (col_num % 2 == 0) {
        throw Exception("column number must be odd: "
                + boost::lexical_cast<std::string>(col_num));
    }

    row_num_ = row_num;
    col_num_ = col_num;

    bg_.set_size(row_num_, col_num_);
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

int Board::add_pawn(std::shared_ptr<Pawn> pawn)
{
    pawn_sides_[pawn] = next_side();
    int n;

    switch (pawn_sides_[pawn]) {
    case 0:
        n = col_num_ / 2;
        break;
    case 1:
        n = (row_num_ / 2) * col_num_;
        break;
    case 2:
        n = (row_num_ - 1) * col_num_ + col_num_ / 2;
        break;
    case 3:
        n = (row_num_ / 2) * col_num_ + col_num_ - 1;
        break;
    default:
        throw Exception("invalid pawn side: "
                + boost::lexical_cast<std::string>(pawn_sides_[pawn]));
    }

    occ_nodes_[n] = pawn;
    pawn_nodes_[pawn] = n;

    bg_.block_neighbours(n);

    return 0;
}

bool Board::is_occupied(int node) const
{
    return occ_nodes_.find(node) != occ_nodes_.end();
}

pos_t Board::pawn_pos(std::shared_ptr<Pawn> pawn) const
{
    int node = pawn_nodes_.at(pawn);
    pos_t pos;
    pos.row = row(node);
    pos.col = col(node);
    return pos;
}

bool Board::is_at_opposite_side(std::shared_ptr<Pawn> pawn) const
{
    int n = pawn_nodes_.at(pawn);
    switch (pawn_sides_.at(pawn)) {
    case 0:
        return row(n) == row_num_ - 1;
    case 1:
        return col(n) == col_num_ - 1;
    case 2:
        return row(n) == 0;
    case 3:
        return col(n) == 0;
    default:
        throw Exception("invalid board side: "
                + boost::lexical_cast<std::string>(pawn_sides_.at(pawn)));
    }
}

int Board::make_walking_move(std::shared_ptr<Pawn> pawn, int goal_node)
{
    int cur_node = pawn_nodes_[pawn];

    if (!is_possible_move(cur_node, goal_node)) {
        return -1;
    }

    // update pawn's position
    occ_nodes_.erase(pawn_nodes_[pawn]);
    bg_.unblock_neighbours(pawn_nodes_[pawn]);
    pawn_nodes_[pawn] = goal_node;
    occ_nodes_[goal_node] = pawn;
    bg_.block_neighbours(goal_node);

    return 0;
}

int Board::add_wall(const Wall &wall)
{
    if (try_add_wall(wall) < 0) {
        return -1;
    }

    walls_[wall.orientation()][wall.line()].insert(std::map<int, Wall>::value_type(wall.start_pos(), Wall(wall)));

    int node1;
    int node2;
    for (int i = 0; i < wall.cnt(); ++i) {
        if (wall.orientation() == 0) {
            node1 = wall.line() * col_num_ + wall.start_pos() + i;
            node2 = (wall.line() + 1) * col_num_ + wall.start_pos() + i;
        }
        else {
            node1 = (wall.start_pos() + i) * row_num_ + wall.line();
            node2 = (wall.start_pos() + i) * row_num_ + wall.line() + 1;
        }
        bg_.remove_edges(node1, node2);
    }

    return 0;
}

int Board::try_add_wall(const Wall &wall)
{
    int line_lim = (wall.orientation() ? col_num() : row_num()) - 1;
    int start_pos_lim = (wall.orientation() ? row_num() : col_num()) - 1;
    if ((wall.line() >= line_lim)
            || (wall.end_pos() > start_pos_lim)) {
        return -1;
    }

    if (wall_intersects(wall)) {
        return -1;
    }

    bg_.reset_filters();

    int node1;
    int node2;
    for (int i = 0; i < wall.cnt(); ++i) {
        if (wall.orientation() == 0) {
            node1 = wall.line() * col_num_ + wall.start_pos() + i;
            node2 = (wall.line() + 1) * col_num_ + wall.start_pos() + i;
        }
        else {
            node1 = (wall.start_pos() + i) * row_num_ + wall.line();
            node2 = (wall.start_pos() + i) * row_num_ + wall.line() + 1;
        }

        bg_.filter_edges(node1, node2);
    }

    bool path_blocked = false;

    for (auto pawn_node : pawn_nodes_) {
        std::vector<int> nodes;
        int side = (pawn_sides_[pawn_node.first] + 2) % 4;
        path_blocked = true;

        side_nodes(side, &nodes);
        for (auto node : nodes) {
            if (bg_.is_path_exists(pawn_node.second, node)) {
                path_blocked = false;
                break;
            }
        }

        /* wall blocks all pathes to the opposite side for one of pawns */
        if (path_blocked) {
            break;
        }
    }

    if (path_blocked) {
        return -1;
    }

    return 0;
}

bool Board::is_possible_move(int cur_node, int goal_node) const
{
    return bg_.is_neighbours(cur_node, goal_node);
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

void Board::side_nodes(int side, std::vector<int> *nodes) const
{
    switch (side) {
    case 0:
        for (int i = 0; i < col_num_; ++i) {
            nodes->push_back(i);
        }
        break;
    case 1:
        for (int i = 0; i < row_num_; ++i) {
            nodes->push_back(i * col_num_);
        }
        break;
    case 2:
        for (int i = 0; i < col_num_; ++i) {
            nodes->push_back((row_num_ - 1 ) * col_num_ + i);
        }
        break;
    case 3:
        for (int i = 0; i < row_num_; ++i) {
            nodes->push_back(i * col_num_ + col_num_ - 1);
        }
        break;
    default:
        break;
    }
}

}  /* namespace Quoridor */
