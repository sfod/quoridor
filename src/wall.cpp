#include "wall.hpp"

namespace Quoridor {

Wall::Wall() : orientation_(), row_(), col_(), cnt_()
{
}

Wall::Wall(Orientation orientation, int row, int col, int cnt)
    : orientation_(orientation), row_(row), col_(col), cnt_(cnt)
{
}

Wall::~Wall()
{
}

}  /* namespace Quoridor */
