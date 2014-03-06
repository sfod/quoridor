#ifndef QUORIDOR_WALK_MOVE_HPP_
#define QUORIDOR_WALK_MOVE_HPP_

#include "imove.hpp"
#include "pos.hpp"

namespace Quoridor {

class WalkMove : public IMove {
public:
    WalkMove();
    explicit WalkMove(Pos node);
    virtual ~WalkMove();

    void set_node(Pos node) { node_ = node; }
    Pos node() const { return node_; }

private:
    Pos node_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_WALK_MOVE_HPP_ */
