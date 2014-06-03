#include "player.hpp"
#include <iostream>
#include "walk_move.hpp"

namespace Quoridor {

Player::Player(std::shared_ptr<Game> /* game */, std::shared_ptr<Pawn> /* pawn */)
{
}

Player::~Player()
{
}

IMove *Player::get_move()
{
    IMove *move = new WalkMove();
    return move;
}

}  /* namespace Quoridor */
