#ifndef QUORIDOR_BOARD_HPP_
#define QUORIDOR_BOARD_HPP_

#include <cstdlib>

#include <set>
#include <utility>
#include <vector>


typedef std::pair<int, int> pos_t;

namespace Quoridor {

class Board {
public:
    Board(int row_num, int col_num);
    virtual ~Board();

    void set_size(int row_num, int col_num);
    int row_num() const { return row_num_; }
    int col_num() const { return col_num_; }
    int next_side();

    void add_occupied(const pos_t &pos);
    void rm_occupied(const pos_t &pos);

    int make_move(int move, pos_t cur_pos, pos_t *fin_pos);
    bool is_at_opposite_side(int side, const pos_t &pos);

private:
    int row_num_;
    int col_num_;
    std::set<pos_t> occ_fields_;
    std::vector<pos_t> sides_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_BOARD_HPP_ */
