#ifndef QUORIDOR_IPLAYER_HPP_
#define QUORIDOR_IPLAYER_HPP_

#include <functional>
#include <boost/variant.hpp>
#include "node.hpp"
#include "wall.hpp"

namespace Quoridor {

typedef boost::variant<boost::blank, Node, Wall> move_t;

class IPlayer {
public:
    virtual ~IPlayer() {}
    virtual void get_move(std::function<void(move_t)> callback) = 0;
    virtual bool is_interactive() const = 0;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_IPLAYER_HPP_ */
