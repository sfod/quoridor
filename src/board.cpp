#include "board.hpp"

#include "exception.hpp"


namespace Quoridor {

Board::Board(size_t row_num, size_t col_num)
    : row_num_(row_num), col_num_(col_num), occ_fields_()
{
    if (row_num == 0) {
        throw Exception();
    }
    if (col_num == 0) {
        throw Exception();
    }
}

Board::~Board()
{
}

void Board::set_size(size_t row_num, size_t col_num)
{
    row_num_ = row_num;
    col_num_ = col_num;
}

void Board::add_occupied(int row, int col)
{
    std::pair<int, int> field(row, col);
    if (occ_fields_.count(field) > 0) {
        throw Exception();
    }
    occ_fields_.insert(std::pair<int, int>(row, col));
}

void Board::rm_occupied(int row, int col)
{
    std::pair<int, int> field(row, col);
    occ_fields_.erase(field);
}

}  /* namespace Quoridor */
