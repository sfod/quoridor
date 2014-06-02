#ifndef QUORIDOR_GAME_HPP_
#define QUORIDOR_GAME_HPP_

#include <map>
#include <memory>
#include <set>
#include <vector>

#include "board_graph.hpp"
#include "node.hpp"
#include "pawn.hpp"
#include "walk_move.hpp"
#include "wall_grid.hpp"
#include "wall_move.hpp"

namespace Quoridor {

struct pawn_data_t {
    std::shared_ptr<Pawn> pawn;
    Node node;
    std::set<Node> goal_nodes;
};

class Game {
public:
    explicit Game(int board_size);
    virtual ~Game();

    void set_pawns(std::vector<std::shared_ptr<Pawn>> &pawn_list);
    std::shared_ptr<Pawn> next_pawn();

    int move_pawn(const WalkMove &move);
    int add_wall(const WallMove &move);

    bool is_finished() const;

private:
    int try_add_wall(const Wall &wall,
            std::vector<std::pair<Node, Node>> *edges);

private:
    int board_size_;
    std::map<int, pawn_data_t> pawn_data_list_;
    int cur_pawn_idx_;
    BoardGraph bg_;
    WallGrid wg_;
};

}  // namespace Quoridor

#endif  // QUORIDOR_GAME_HPP_
