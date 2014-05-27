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
#include "node.hpp"

namespace Quoridor {

class Board {
public:
    explicit Board(int size);
    virtual ~Board();

    int add_pawn(std::shared_ptr<Pawn> pawn);
    bool is_occupied(const Node &node) const;
    Node pawn_node(std::shared_ptr<Pawn> pawn) const;

    int make_walking_move(std::shared_ptr<Pawn> pawn, const Node &node);
    bool is_at_goal_node(std::shared_ptr<Pawn> pawn) const;

    int add_wall(const Wall &wall);
    int try_add_wall(const Wall &wall,
            std::vector<std::pair<Node, Node>> *edges);

    void pawn_goal_nodes(std::shared_ptr<Pawn> pawn,
            std::vector<Node> *nodes) const;
    bool get_path(std::shared_ptr<Pawn> pawn, const Node &node,
            std::list<Node> *nodes) const;

private:
    int next_side() const;
    int row(int n) const { return n / size_; }
    int col(int n) const { return n % size_; }
    bool is_possible_move(const Node &cur_node, const Node &node) const;
    bool wall_intersects(const Wall &wall) const;
    void side_nodes(int side, std::vector<Node> *nodes) const;

private:
    int size_;
    mutable std::vector<std::pair<int, int>> sides_;
    std::map<std::shared_ptr<Pawn>, int> pawn_sides_;
    std::map<int, std::map<int, std::map<int, Wall>>> walls_;

    std::map<Node, std::shared_ptr<Pawn>> occ_nodes_;
    std::map<std::shared_ptr<Pawn>, Node> pawn_nodes_;
    BoardGraph bg_;
};

}  // namespace Quoridor

#endif  // QUORIDOR_BOARD_HPP_
