#include "board.hpp"

#include <cstring>

#include "exception.hpp"


namespace Quoridor {


Wall::Wall(int orientation, int line, int start_pos, int cnt)
    : orientation_(orientation), line_(line), start_pos_(start_pos), cnt_(cnt)
{
}

Wall::~Wall()
{
}


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
        throw Exception();
    }
    if (col_num <= 0) {
        throw Exception();
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
        throw Exception();
    }

    occ_fields_[pos] = pawn;
    pawn_pos_[pawn] = pos;

    return 0;
}

void Board::add_occupied(const pos_t &pos, std::shared_ptr<Pawn> pawn)
{
    if (occ_fields_.count(pos) > 0) {
        throw Exception();
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

int Board::make_move(BoardMoves move, std::shared_ptr<Pawn> pawn)
{
    if (move < kPutWall) {
        return make_walking_move(move, pawn);
    }
    else if (move == kPutWall) {
        return -1;
    }
    else {
        return -1;
    }
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
        throw Exception();
    }
}

BoardMoves Board::recalc_move(BoardMoves move, std::shared_ptr<Pawn> pawn)
{
    int m = (move + pawn_sides_[pawn]) % 4;
    if (m >= kEND) {
        return kEND;
    }
    return static_cast<BoardMoves>(m);
}

int Board::make_walking_move(BoardMoves move, std::shared_ptr<Pawn> pawn)
{
    move = recalc_move(move, pawn);
    pos_t pos = pawn_pos_[pawn];
    pos_t inc_pos;
    pos_t lim_pos = pos;

    switch (move) {
    case kForward:
        lim_pos.row = row_num() - 1;
        inc_pos.row = 1;
        break;
    case kRight:
        lim_pos.col = col_num() - 1;
        inc_pos.col = 1;
        break;
    case kBackward:
        lim_pos.row = 0;
        inc_pos.row = -1;
        break;
    case kLeft:
        lim_pos.col = 0;
        inc_pos.col = -1;
        break;
    case kPutWall:
    case kEND:
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
                return -1;
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
    int start_pos_lim = wall.orientation() ? row_num() : col_num();
    if ((wall.line() >= line_lim)
            || (wall.start_pos() + wall.cnt() >= start_pos_lim)) {
        return -1;
    }

    if (wall_intersects(wall)) {
        return -2;
    }

    walls_[wall.orientation()].insert(std::map<int, Wall>::value_type(wall.line(), Wall(wall)));

    return 0;
}

bool Board::wall_intersects(const Wall &wall) const
{
    // check intersections
    if (walls_.count(1 - wall.orientation()) > 0) {
        for (int i = 1; i < wall.cnt(); ++i) {
            if (walls_.at(1 - wall.orientation()).count(wall.start_pos() - i) != 0) {
                return true;
            }
        }
    }
    // check overlaps
    if (walls_.count(wall.orientation()) > 0) {
        for (int i = 1 - wall.cnt(); i < wall.cnt(); ++i) {
            if (walls_.at(wall.orientation()).count(wall.start_pos() + i) != 0) {
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

    if (inc_pos.row != 0) {
        orientation = 1;
        st = pos.col;
    }
    else if (inc_pos.col != 0) {
        orientation = 0;
        st = pos.row;
    }
    else {
        throw Exception();
    }

    if (walls_.count(orientation) == 0) {
        return true;
    }

    for (int i = 0; i <= st; ++i) {
        if (walls_.at(orientation).count(i)) {
            if (i + walls_.at(orientation).at(i).cnt() >= st) {
                return false;
            }
        }
    }

    return true;
}

}  /* namespace Quoridor */
