#ifndef QUORIDOR_IPLAYER_HPP_
#define QUORIDOR_IPLAYER_HPP_

#include <boost/variant.hpp>
#include "node.hpp"
#include "wall.hpp"

namespace Quoridor {

typedef boost::variant<boost::blank, Node, Wall> move_t;

class IPlayer {
public:
    virtual ~IPlayer() {}
    virtual move_t get_move() = 0;
    virtual bool is_interactive() const = 0;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_IPLAYER_HPP_ */
