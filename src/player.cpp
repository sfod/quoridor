#include "player.hpp"
#include <iostream>
#include "walk_move.hpp"

namespace Quoridor {

Player::Player()
{
}

Player::~Player()
{
}

IMove *Player::get_move()
{
    int i;
    std::cin >> i;

    IMove *move = new WalkMove(i);

    return move;
}

}  /* namespace Quoridor */
