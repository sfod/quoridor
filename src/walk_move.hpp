#ifndef QUORIDOR_WALK_MOVE_HPP_
#define QUORIDOR_WALK_MOVE_HPP_

#include "imove.hpp"

namespace Quoridor {

class WalkMove : public IMove {
public:
    WalkMove();
    explicit WalkMove(int dir);
    virtual ~WalkMove();

    int dir() const { return dir_; }
    void set_dir(int dir) { dir_ = dir; }

public:
    enum Direction {
        kStart = 0,
        kForward = 1,
        kRight,
        kBackward,
        kLeft,
        kEnd,
    };

private:
    int dir_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_WALK_MOVE_HPP_ */
