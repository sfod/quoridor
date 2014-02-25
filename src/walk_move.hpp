#ifndef QUORIDOR_WALK_MOVE_HPP_
#define QUORIDOR_WALK_MOVE_HPP_

#include "board.hpp"
#include "imove.hpp"

namespace Quoridor {

class WalkMove : public IMove {
public:
    WalkMove();
    explicit WalkMove(int node);
    virtual ~WalkMove();

    void set_node(int node) { node_ = node; }
    int node() const { return node_; }

private:
    int node_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_WALK_MOVE_HPP_ */
