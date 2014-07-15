#include "player.hpp"
#include "exception.hpp"

namespace Quoridor {

void Player::get_move(std::function<void(move_t)> /* callback */)
{
    throw Exception("method get_move() is called on Player");
}

}  /* namespace Quoridor */
