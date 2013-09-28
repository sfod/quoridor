#pragma once

#include <cstdlib>

#include <set>
#include <utility>
#include <vector>


typedef std::pair<int, int> pos_t;

namespace Quoridor {

class Board {
public:
    Board(size_t row_num, size_t col_num);
    virtual ~Board();

    void set_size(size_t row_num, size_t col_num);
    size_t row_num() const { return row_num_; }
    size_t col_num() const { return col_num_; }
    int next_side();

    void add_occupied(const pos_t &pos);
    void rm_occupied(const pos_t &pos);

    int make_move(int move, pos_t cur_pos, pos_t *fin_pos);
    bool is_at_opposite_side(int side, const pos_t &pos);

private:
    size_t row_num_;
    size_t col_num_;
    std::set<pos_t> occ_fields_;
    std::vector<pos_t> sides_;
};

}  /* namespace Quoridor */
