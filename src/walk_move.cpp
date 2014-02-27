#include "walk_move.hpp"
#include <boost/lexical_cast.hpp>
#include "exception.hpp"

namespace Quoridor {

WalkMove::WalkMove() : node_()
{
}

WalkMove::WalkMove(int node) : node_(node)
{
}

WalkMove::~WalkMove()
{
}

}  /* namespace Quoridor */
