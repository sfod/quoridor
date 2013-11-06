#include "walk_move.hpp"
#include <boost/lexical_cast.hpp>
#include "exception.hpp"

namespace Quoridor {

WalkMove::WalkMove() : dir_(kEnd)
{
}

WalkMove::WalkMove(int dir)
{
    if ((dir <= Direction::kStart) || (dir >= Direction::kEnd)) {
        throw Exception("invalid direction: "
                + boost::lexical_cast<std::string>(dir));
    }
    dir_ = dir;
}

WalkMove::~WalkMove()
{
}

}  /* namespace Quoridor */
