#ifndef QUORIDOR_IPLAYER_HPP_
#define QUORIDOR_IPLAYER_HPP_

#include "imove.hpp"

namespace Quoridor {

class IPlayer {
public:
    virtual ~IPlayer() {}
    virtual IMove *get_move() = 0;
    virtual bool is_interactive() const = 0;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_IPLAYER_HPP_ */
