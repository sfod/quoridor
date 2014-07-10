#ifndef QUORIDOR_PLAYER_HPP_
#define QUORIDOR_PLAYER_HPP_

#include <memory>
#include "iplayer.hpp"
#include "game.hpp"
#include "pawn.hpp"

namespace Quoridor {

class Player : public IPlayer {
public:
    virtual move_t get_move();
    virtual bool is_interactive() const { return true; }
};

}  /* namespace Quoridor */

#endif  // QUORIDOR_PLAYER_HPP_
