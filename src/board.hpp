#pragma once

#include <cstdlib>

#include <set>
#include <utility>

namespace Quoridor {

class Board {
public:
    Board(size_t row_num, size_t col_num);
    virtual ~Board();

    void set_size(size_t row_num, size_t col_num);

    void add_occupied(int row, int col);
    void rm_occupied(int row, int col);

private:
    size_t row_num_;
    size_t col_num_;
    std::set<std::pair<int, int>> occ_fields_;
};

}  /* namespace Quoridor */
