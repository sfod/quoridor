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
        pos.first = 0;
        pos.second = 4;
        break;
    case 1:
        pos.first = 4;
        pos.second = 0;
        break;
    case 2:
        pos.first = 8;
        pos.second = 4;
        break;
    case 3:
        pos.first = 4;
        pos.second = 8;
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
        return pawn_pos_.at(pawn).first == row_num() - 1;
    case 1:
        return pawn_pos_.at(pawn).second == col_num() - 1;
    case 2:
        return pawn_pos_.at(pawn).first == 0;
    case 3:
        return pawn_pos_.at(pawn).second == 0;
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

    int *ch_pos = NULL;
    int lim = -1;
    int inc = 0;

    switch (move) {
    case kForward:
        ch_pos = &pos.first;
        lim = row_num() - 1;
        inc = 1;
        break;
    case kRight:
        ch_pos = &pos.second;
        lim = col_num() - 1;
        inc = 1;
        break;
    case kBackward:
        ch_pos = &pos.first;
        lim = 0;
        inc = -1;
        break;
    case kLeft:
        ch_pos = &pos.second;
        lim = 0;
        inc = -1;
        break;
    case kPutWall:
    case kEND:
    default:
        return -1;
    }

    // error, pawn is already at the opposite side
    if (*ch_pos == lim) {
        return -1;
    }

    (*ch_pos) += inc;
    // the next field is occupied
    if (occ_fields_.count(pos) > 0) {
        // if the field after the next is also occupied or there is a wall,
        // pawn may go to the left or to the right of the next field
        (*ch_pos) += inc;
        if (occ_fields_.count(pos) > 0) {
        }
        else {}
    }

    do {
        (*ch_pos) += inc;
    } while (occ_fields_.count(pos) > 0);

    // pawn cannot make specified move
    if (((lim > 0) && (*ch_pos > lim)) || ((lim == 0) && (*ch_pos < lim))) {
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

}  /* namespace Quoridor */
