#include "wall.hpp"
#include <map>
#include <boost/assign/list_of.hpp>

namespace Quoridor {

std::map<Wall::Orientation, const char*> orientation_str = boost::assign::map_list_of
    (Wall::Orientation::kHorizontal, "horizontal")
    (Wall::Orientation::kVertical, "vertical")
;

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

std::ostream &operator<<(std::ostream &os, const Quoridor::Wall &wall)
{
    return os << orientation_str[wall.orientation()] << " wall at " << wall.row() << ":"
        << wall.col() << " (" << wall.cnt() << ")";
}

}  /* namespace Quoridor */
