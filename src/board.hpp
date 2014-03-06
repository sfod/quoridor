#ifndef QUORIDOR_BOARD_HPP_
#define QUORIDOR_BOARD_HPP_

#include <cstdlib>

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "board_graph.hpp"
#include "pawn.hpp"
#include "wall.hpp"
#include "pos.hpp"

namespace Quoridor {

class Board {
public:
    Board(int row_num, int col_num);
    virtual ~Board();

    void set_size(int row_num, int col_num);
    int row_num() const { return row_num_; }
    int col_num() const { return col_num_; }

    int add_pawn(std::shared_ptr<Pawn> pawn);
    bool is_occupied(const Pos &node) const;
    Pos pawn_node(std::shared_ptr<Pawn> pawn) const;

    int make_walking_move(std::shared_ptr<Pawn> pawn, const Pos &node);
    bool is_at_goal_node(std::shared_ptr<Pawn> pawn) const;

    int add_wall(const Wall &wall);
    int try_add_wall(const Wall &wall,
            std::vector<std::pair<Pos, Pos>> *edges);

    void pawn_goal_nodes(std::shared_ptr<Pawn> pawn,
            std::vector<Pos> *nodes) const;
    bool get_path(std::shared_ptr<Pawn> pawn, const Pos &node,
            std::list<Pos> *nodes) const;

private:
    int next_side() const;
    int row(int n) const { return n / col_num_; }
    int col(int n) const { return n % col_num_; }
    bool is_possible_move(const Pos &cur_node, const Pos &node) const;
    bool wall_intersects(const Wall &wall) const;
    void side_nodes(int side, std::vector<Pos> *nodes) const;

private:
    int row_num_;
    int col_num_;
    mutable std::vector<std::pair<int, int>> sides_;
    std::map<std::shared_ptr<Pawn>, int> pawn_sides_;
    std::map<int, std::map<int, std::map<int, Wall>>> walls_;

    std::map<Pos, std::shared_ptr<Pawn>> occ_nodes_;
    std::map<std::shared_ptr<Pawn>, Pos> pawn_nodes_;
    BoardGraph bg_;
};

}  // namespace Quoridor

#endif  // QUORIDOR_BOARD_HPP_
