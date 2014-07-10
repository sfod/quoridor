#include "player.hpp"
#include "exception.hpp"

namespace Quoridor {

move_t Player::get_move()
{
    throw Exception("method get_move() is called on Player");
    return boost::blank();
}

}  /* namespace Quoridor */
