#ifndef QUORIDOR_BOARD_HPP_
#define QUORIDOR_BOARD_HPP_

#include <cstdlib>

#include <map>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "pawn.hpp"
#include "wall.hpp"


namespace Quoridor {

struct pos_t {
    pos_t() : row(0), col(0) {}

    int row;
    int col;

    bool operator<(const pos_t &pos) const {
        return std::tie(row, col) < std::tie(pos.row, pos.col);
    }
    bool operator==(const pos_t &pos) const {
        return (row == pos.row) && (col == pos.col);
    }
    const pos_t &operator+=(const pos_t &pos) {
        row += pos.row;
        col += pos.col;
        return *this;
    }
    const pos_t operator+(const pos_t &pos) {
        pos_t p = *this;
        p += pos;
        return p;
    }
};

class Board {
public:
    Board(int row_num, int col_num);
    virtual ~Board();

    void set_size(int row_num, int col_num);
    int row_num() const { return row_num_; }
    int col_num() const { return col_num_; }
    int next_side() const;

    int add_pawn(std::shared_ptr<Pawn> pawn);
    void add_occupied(const pos_t &pos, std::shared_ptr<Pawn> pawn);
    void rm_occupied(const pos_t &pos);
    pos_t pawn_pos(std::shared_ptr<Pawn> pawn) const;

    int make_walking_move(int dir, std::shared_ptr<Pawn> pawn);
    bool is_at_opposite_side(std::shared_ptr<Pawn> pawn) const;
    int add_wall(const Wall &wall);

private:
    int recalc_dir(int dir, std::shared_ptr<Pawn> pawn);
    bool wall_intersects(const Wall &wall) const;
    bool is_outside_board(const pos_t &pos) const;
    bool is_possible_move(const pos_t &pos, const pos_t &inc_pos) const;

private:
    int row_num_;
    int col_num_;
    std::map<pos_t, std::shared_ptr<Pawn>> occ_fields_;
    std::map<std::shared_ptr<Pawn>, pos_t> pawn_pos_;
    mutable std::vector<std::pair<int, int>> sides_;
    std::map<std::shared_ptr<Pawn>, int> pawn_sides_;
    std::map<int, std::map<int, Wall>> walls_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_BOARD_HPP_ */
