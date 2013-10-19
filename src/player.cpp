#include "player.hpp"
#include <iostream>

namespace Quoridor {

Player::Player()
{
}

Player::~Player()
{
}

int Player::get_move(BoardMoves *move)
{
    int i;
    std::cin >> i;

    if (i >= kEND) {
        return -1;
    }

    *move = static_cast<BoardMoves>(i);

    return 0;
}

}  /* namespace Quoridor */
