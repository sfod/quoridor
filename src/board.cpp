#include "board.hpp"
#include <cstring>
#include "exception.hpp"


namespace Quoridor {

Board::Board(int row_num, int col_num)
    : row_num_(row_num), col_num_(col_num), occ_fields_()
{
    if (row_num == 0) {
        throw Exception();
    }
    if (col_num == 0) {
        throw Exception();
    }
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
    row_num_ = row_num;
    col_num_ = col_num;
}

int Board::next_side()
{
    for (auto &side : sides_) {
        if (side.second == 0) {
            side.second = 1;
            return side.first;
        }
    }
    return -1;
}

void Board::add_occupied(const pos_t &pos)
{
    if (occ_fields_.count(pos) > 0) {
        throw Exception();
    }
    occ_fields_.insert(pos);
}

void Board::rm_occupied(const pos_t &pos)
{
    occ_fields_.erase(pos);
}

int Board::make_move(int move, pos_t cur_pos, pos_t *fin_pos)
{
    pos_t pos = cur_pos;

    switch (move) {
    case 0:
        if (cur_pos.first == (int) row_num_ - 1) {
            return -1;
        }
        do {
            pos.first++;
        } while (occ_fields_.count(pos) > 0);
        if (pos.first > (int) row_num_ - 1) {
            return -1;
        }
        *fin_pos= pos;
        break;
    case 1:
        if (cur_pos.second == (int) col_num_ - 1) {
            return -1;
        }
        do {
            pos.second++;
        } while (occ_fields_.count(pos) > 0);
        if (pos.second > (int) col_num_ - 1) {
            return -1;
        }
        *fin_pos= pos;
        break;
    case 2:
        if (cur_pos.first == 0) {
            return -1;
        }
        do {
            pos.first--;
        } while (occ_fields_.count(pos) > 0);
        if (pos.first < 0) {
            return -1;
        }
        *fin_pos= pos;
        break;
    case 3:
        if (cur_pos.second == 0) {
            return -1;
        }
        do {
            pos.second--;
        } while (occ_fields_.count(pos) > 0);
        if (pos.second < 0) {
            return -1;
        }
        *fin_pos= pos;
        break;
    }

    return 0;
}

bool Board::is_at_opposite_side(int side, const pos_t &pos)
{
    if (side == 0) {
        return pos.first == row_num() - 1;
    }
    if (side == 1) {
        return pos.second == col_num() - 1;
    }
    if (side == 2) {
        return pos.first == 0;
    }
    if (side == 3) {
        return pos.second == 0;
    }
    return false;
}

}  /* namespace Quoridor */
