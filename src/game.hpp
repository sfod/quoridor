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
    void make_move(Player *player);
    bool is_win(const Player *player) const;

private:
    Board *board_;
    std::vector<Player*> player_list_;
};

}  /* namespace Quoridor */
