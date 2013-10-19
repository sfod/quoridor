#ifndef QUORIDOR_IPLAYER_HPP_
#define QUORIDOR_IPLAYER_HPP_

#include "game.hpp"

namespace Quoridor {

class IPlayer {
public:
    virtual ~IPlayer() {}
    virtual int get_move(BoardMoves *move) = 0;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_IPLAYER_HPP_ */
