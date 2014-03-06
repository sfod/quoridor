#include "walk_move.hpp"

namespace Quoridor {

WalkMove::WalkMove() : node_()
{
}

WalkMove::WalkMove(Pos node) : node_(node)
{
}

WalkMove::~WalkMove()
{
}

}  /* namespace Quoridor */
