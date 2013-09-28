#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "board.hpp"
#include "player.hpp"

namespace Quoridor {

class Game {
public:
    explicit Game(std::shared_ptr<Quoridor::Board> board);
    virtual ~Game();

    void main_loop();

    void add_player(std::shared_ptr<Player> player);
    void make_move(std::shared_ptr<Player> player);
    bool is_win(std::shared_ptr<Player> player) const;

private:
    std::shared_ptr<Quoridor::Board> board_;
    std::vector<std::shared_ptr<Player>> player_list_;
};

}  /* namespace Quoridor */
