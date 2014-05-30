#ifndef QUORIDOR_WALL_MOVE_HPP_
#define QUORIDOR_WALL_MOVE_HPP_

#include "imove.hpp"
#include "wall.hpp"

namespace Quoridor {

class WallMove : public IMove {
public:
    explicit WallMove(const Wall &wall);
    virtual ~WallMove();

    Wall wall() const { return wall_; }
    void set_wall(const Wall &wall) { wall_ = wall; }

private:
    Wall wall_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_WALL_MOVE_HPP_ */
