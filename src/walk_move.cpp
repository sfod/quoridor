#include "walk_move.hpp"
#include "exception.hpp"

namespace Quoridor {

WalkMove::WalkMove() : dir_(kEnd)
{
}

WalkMove::WalkMove(int dir)
{
    if ((dir <= Direction::kStart) || (dir >= Direction::kEnd)) {
        throw Exception("invalid direction");
    }
    dir_ = dir;
}

WalkMove::~WalkMove()
{
}

}  /* namespace Quoridor */
