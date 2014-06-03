#include "wall_grid.hpp"

namespace Quoridor {

enum NodeOccupiedSide {
    kBottom = 1,
    kLeft = 2,
    kRight = 4,
    kUp = 8,
    kVertical = kBottom | kUp,
    kHorizontal = kLeft | kRight,
    kAll = 15
};

static const NodeOccupiedSide horiz_occ_sides_[] = {
    NodeOccupiedSide::kRight,
    NodeOccupiedSide::kHorizontal,
    NodeOccupiedSide::kLeft,
};
static const NodeOccupiedSide vert_occ_sides_[] = {
    NodeOccupiedSide::kUp,
    NodeOccupiedSide::kVertical,
    NodeOccupiedSide::kBottom,
};

WallGrid::WallGrid(int size) : occupied_nodes_(), tmp_occupied_nodes_(),
    size_(size)
{
}

WallGrid::~WallGrid()
{
}

int WallGrid::add_wall(const Wall &wall)
{
    if (add_tmp_wall(wall) == 0) {
        apply_tmp_wall();
        return 0;
    }
    return -1;
}

int WallGrid::add_tmp_wall(const Wall &wall)
{
    int rc = 0;

    // check if wall is set on the border
    if ((wall.orientation() == Wall::kHorizontal)
            && ((wall.row() == 0) || (wall.row() == size_ - 1))) {
        return -1;
    }
    else if ((wall.orientation() == Wall::kVertical)
            && ((wall.col() == 0) || (wall.col() == size_ - 1))) {
        return -1;
    }

    Node inc_node(0, 0);
    const NodeOccupiedSide *occ_sides;

    switch (wall.orientation()) {
    case Wall::kHorizontal:
        inc_node.set_col(1);
        occ_sides = horiz_occ_sides_;
        break;
    case Wall::kVertical:
        inc_node.set_row(1);
        occ_sides = vert_occ_sides_;
        break;
    case Wall::kInvalid:
    default:
        return -1;
    }

    tmp_occupied_nodes_.clear();

    Node node(wall.row(), wall.col());
    NodeOccupiedSide occ_side;
    for (int i = 0; i <= wall.cnt(); ++i) {
        // wall extends outside the board
        if ((node.row() > size_) || (node.col() > size_)) {
            rc = -1;
            break;
        }

        if (i == 0) {
            occ_side = occ_sides[0];
        }
        else if (i < wall.cnt()) {
            occ_side = occ_sides[1];
        }
        else {
            occ_side = occ_sides[2];
        }

        // new wall intersects with earlier added wall
        if ((occupied_nodes_.count(node) != 0)
                && (((occupied_nodes_[node] & occ_side) != 0)
                    || ((occupied_nodes_[node] | occ_side) == NodeOccupiedSide::kAll))) {
            rc = -1;
            break;
        }
        else {
            tmp_occupied_nodes_[node] = occ_side;
        }

        node += inc_node;
    }

    return rc;
}

void WallGrid::apply_tmp_wall()
{
    for (auto it : tmp_occupied_nodes_) {
        occupied_nodes_[it.first] |= it.second;
    }
    tmp_occupied_nodes_.clear();
}

}  // namespace Quoridor
