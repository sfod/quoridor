#include "player.hpp"
#include <iostream>
#include "walk_move.hpp"

namespace Quoridor {

Player::Player(std::shared_ptr<Board> /* board */, std::shared_ptr<Pawn> /* pawn */)
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
