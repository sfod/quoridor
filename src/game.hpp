#pragma once

#include <utility>
#include <vector>

#include "board.hpp"
#include "player.hpp"

namespace Quoridor {

class Game {
public:
    explicit Game(Board *board);
    virtual ~Game();

    void main_loop();

    void add_player(Player *player);
    void make_move(std::pair<Player*, int> &pl_side);

private:
    Board *board_;
    std::vector<std::pair<Player*, int>> player_list_;
};

}  /* namespace Quoridor */
