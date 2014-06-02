#include "wall.hpp"

namespace Quoridor {

Wall::Wall() : orientation_(), line_(), start_pos_(), cnt_()
{
}

Wall::Wall(Orientation orientation, int line, int start_pos, int cnt)
    : orientation_(orientation), line_(line), start_pos_(start_pos), cnt_(cnt)
{
}

Wall::~Wall()
{
}

}  /* namespace Quoridor */
