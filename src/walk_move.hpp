#ifndef QUORIDOR_WALK_MOVE_HPP_
#define QUORIDOR_WALK_MOVE_HPP_

#include "imove.hpp"
#include "node.hpp"

namespace Quoridor {

class WalkMove : public IMove {
public:
    WalkMove();
    explicit WalkMove(Node node);
    virtual ~WalkMove();

    void set_node(Node node) { node_ = node; }
    Node node() const { return node_; }

private:
    Node node_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_WALK_MOVE_HPP_ */
