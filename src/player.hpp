#pragma once

#include <string>
#include "board.hpp"

namespace Quoridor {

class Player {
public:
    Player(const std::string &name, Board *board);
    virtual ~Player();

    const std::string& name() const { return name_; }
    int make_move();

private:
    int move_forward();
    int move_back();
    int move_right();
    int move_left();

private:
    std::string name_;
    Board *board_;
    int cur_row_;
    int cur_col_;
};

}  /* namespace Quoridor */
