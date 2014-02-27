#ifndef QUORIDOR_BOARD_HPP_
#define QUORIDOR_BOARD_HPP_

#include <cstdlib>

#include <map>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "board_graph.hpp"
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
    bool is_occupied(int node) const;
    pos_t pawn_pos(std::shared_ptr<Pawn> pawn) const;

    int make_walking_move(std::shared_ptr<Pawn> pawn, int goal_node);
    bool is_at_opposite_side(std::shared_ptr<Pawn> pawn) const;
    int add_wall(const Wall &wall);
    int try_add_wall(const Wall &wall, std::vector<std::pair<int, int>> *edges);

    void pawn_final_nodes(std::shared_ptr<Pawn> pawn,
            std::vector<int> *nodes) const;
    bool get_path(std::shared_ptr<Pawn> pawn, int end_node,
            std::list<int> *nodes) const;

private:
    int row(int n) const { return n / col_num_; }
    int col(int n) const { return n % col_num_; }

private:
    bool is_possible_move(int cur_node, int goal_node) const;
    bool wall_intersects(const Wall &wall) const;
    void side_nodes(int side, std::vector<int> *nodes) const;

private:
    int row_num_;
    int col_num_;
    mutable std::vector<std::pair<int, int>> sides_;
    std::map<std::shared_ptr<Pawn>, int> pawn_sides_;
    std::map<int, std::map<int, std::map<int, Wall>>> walls_;

    std::map<int, std::shared_ptr<Pawn>> occ_nodes_;
    std::map<std::shared_ptr<Pawn>, int> pawn_nodes_;
    BoardGraph bg_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_BOARD_HPP_ */
