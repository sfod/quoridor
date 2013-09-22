#pragma once

#include <vector>

#include "board.hpp"
#include "player.hpp"

namespace Quoridor {

class Game {
public:
    explicit Game(const Board &board);
    virtual ~Game();

    void add_player(const Player &player);
    void main_loop();
    void make_move(Player &player);

private:
    Board board_;
    std::vector<Player> player_list_;
};

}  /* namespace Quoridor */
