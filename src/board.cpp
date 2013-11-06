#include "board.hpp"

#include <cstring>

#include <boost/lexical_cast.hpp>

#include "exception.hpp"
#include "walk_move.hpp"


namespace Quoridor {

Board::Board(int row_num, int col_num) : row_num_(), col_num_(),
        occ_fields_(), pawn_pos_(), sides_(), pawn_sides_(), walls_()
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
    row_num_ = row_num;
    col_num_ = col_num;
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
    pos_t pos;

    switch (pawn_sides_[pawn]) {
    case 0:
        pos.row = 0;
        pos.col = 4;
        break;
    case 1:
        pos.row = 4;
        pos.col = 0;
        break;
    case 2:
        pos.row = 8;
        pos.col = 4;
        break;
    case 3:
        pos.row = 4;
        pos.col = 8;
        break;
    default:
        throw Exception("invalid pawn side: "
                + boost::lexical_cast<std::string>(pawn_sides_[pawn]));
    }

    occ_fields_[pos] = pawn;
    pawn_pos_[pawn] = pos;

    return 0;
}

void Board::add_occupied(const pos_t &pos, std::shared_ptr<Pawn> pawn)
{
    if (occ_fields_.count(pos) > 0) {
        throw Exception("cell (" + boost::lexical_cast<std::string>(pos.row)
                + ":" + boost::lexical_cast<std::string>(pos.col)
                + ") is already occupied");
    }
    occ_fields_[pos] = pawn;
    pawn_pos_[pawn] = pos;
}

void Board::rm_occupied(const pos_t &pos)
{
    occ_fields_.erase(pos);
}

pos_t Board::pawn_pos(std::shared_ptr<Pawn> pawn) const
{
    return pawn_pos_.at(pawn);
}

bool Board::is_at_opposite_side(std::shared_ptr<Pawn> pawn) const
{
    switch (pawn_sides_.at(pawn)) {
    case 0:
        return pawn_pos_.at(pawn).row == row_num() - 1;
    case 1:
        return pawn_pos_.at(pawn).col == col_num() - 1;
    case 2:
        return pawn_pos_.at(pawn).row == 0;
    case 3:
        return pawn_pos_.at(pawn).col == 0;
    default:
        throw Exception("invalid board side: "
                + boost::lexical_cast<std::string>(pawn_sides_.at(pawn)));
    }
}

int Board::recalc_dir(int dir, std::shared_ptr<Pawn> pawn)
{
    int m = (dir + pawn_sides_[pawn]) % 4;
    return m;
}

int Board::make_walking_move(int dir, std::shared_ptr<Pawn> pawn)
{
    dir = recalc_dir(dir, pawn);
    pos_t pos = pawn_pos_[pawn];
    pos_t inc_pos;
    pos_t lim_pos = pos;

    switch (dir) {
        case WalkMove::Direction::kForward:
        lim_pos.row = row_num() - 1;
        inc_pos.row = 1;
        break;
    case WalkMove::Direction::kRight:
        lim_pos.col = col_num() - 1;
        inc_pos.col = 1;
        break;
    case WalkMove::Direction::kBackward:
        lim_pos.row = 0;
        inc_pos.row = -1;
        break;
    case WalkMove::Direction::kLeft:
        lim_pos.col = 0;
        inc_pos.col = -1;
        break;
    case WalkMove::Direction::kEnd:
    default:
        return -1;
    }

    // error, pawn is already at the opposite side
    if (pos == lim_pos) {
        return -1;
    }

    if (is_possible_move(pos, inc_pos)) {
        pos_t possible_pos = pos + inc_pos;
        if (occ_fields_.count(possible_pos) == 0) {
            pos += inc_pos;
        }
        else {
            if (is_possible_move(possible_pos, inc_pos)) {
                pos += inc_pos;
                pos += inc_pos;
            }
            else {
                return -2;
            }
        }
    }
    else {
        return -1;
    }

    // pawn cannot make specified move
    if (is_outside_board(pos)) {
        return -1;
    }

    // update pawn's position
    occ_fields_.erase(pawn_pos_[pawn]);
    pawn_pos_[pawn] = pos;
    occ_fields_[pos] = pawn;

    return 0;
}

int Board::add_wall(const Wall &wall)
{
    int line_lim = wall.orientation() ? col_num() : row_num();
    int start_pos_lim = (wall.orientation() ? row_num() : col_num()) - 1;
    if ((wall.line() >= line_lim)
            || (wall.start_pos() + wall.cnt() >= start_pos_lim)) {
        return -1;
    }

    if (wall_intersects(wall)) {
        return -2;
    }

    walls_[wall.orientation()][wall.line()].insert(std::map<int, Wall>::value_type(wall.start_pos(), Wall(wall)));

    return 0;
}

bool Board::wall_intersects(const Wall &wall) const
{
    std::map<int, Wall> line_walls;
    std::map<int, Wall>::iterator it;

    // check intersections
    if (walls_.count(1 - wall.orientation()) > 0) {
        for (int i = 0; i < wall.cnt() - 1; ++i) {
            // there are some walls on the intersected line
            if (walls_.at(1 - wall.orientation()).count(wall.start_pos()) != 0) {
                line_walls = walls_.at(1 - wall.orientation()).at(wall.start_pos() + i);
                it = line_walls.lower_bound(wall.line());  // the nearest wall lain below the line of new wall
                if ((it != line_walls.end()) && (it->second.start_pos() + it->second.cnt() >= wall.line())) {
                    return true;
                }
            }
        }
    }
    // check overlaps
    if (walls_.count(wall.orientation()) > 0) {
        if (walls_.at(wall.orientation()).count(wall.line()) != 0) {
            line_walls = walls_.at(wall.orientation()).at(wall.line());
            it = line_walls.lower_bound(wall.start_pos());
            if ((it != line_walls.end()) && (it->second.start_pos() + it->second.cnt() >= wall.start_pos())) {
                return true;
            }
        }
    }
    return false;
}

bool Board::is_outside_board(const pos_t &pos) const
{
    if ((pos.row >= row_num()) || (pos.row < 0)
            || (pos.col >= col_num()) || (pos.col < 0))
        return true;
    return false;
}

bool Board::is_possible_move(const pos_t &pos, const pos_t &inc_pos) const
{
    int orientation;
    int st;
    int crossed_line;

    if (inc_pos.row != 0) {
        orientation = 0;
        st = pos.col;
        crossed_line = std::min(pos.row, pos.row + inc_pos.row);
    }
    else if (inc_pos.col != 0) {
        orientation = 1;
        st = pos.row;
        crossed_line = std::min(pos.col, pos.col + inc_pos.col);
    }
    else {
        throw Exception("zero incrementation move");
    }

    if (walls_.count(orientation) == 0) {
        return true;
    }

    if (walls_.at(orientation).count(crossed_line) != 0) {
        std::map<int, Wall> line_walls = walls_.at(orientation).at(crossed_line);
        std::map<int, Wall>::iterator it = line_walls.lower_bound(st);
        if ((it != line_walls.end()) && (it->second.start_pos() + it->second.cnt() >= st)) {
            return false;
        }
    }

    return true;
}

}  /* namespace Quoridor */
