#ifndef QUORIDOR_PLAYER_HPP_
#define QUORIDOR_PLAYER_HPP_

#include <memory>
#include "iplayer.hpp"
#include "game.hpp"
#include "pawn.hpp"

namespace Quoridor {

class Player : public IPlayer {
public:
    virtual void get_move(std::function<void(move_t)> callback);
    virtual bool is_interactive() const { return true; }
    virtual void interrupt() {}
};

}  /* namespace Quoridor */

#endif  // QUORIDOR_PLAYER_HPP_
