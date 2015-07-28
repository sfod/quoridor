#include "wall_grid.hpp"

enum NodeOccupiedSide {
    kBottom = 1,
    kLeft = 2,
    kRight = 4,
    kUp = 8,
    kMiddle = 16,
    kVertical = kMiddle | kBottom | kUp,
    kHorizontal = kMiddle | kLeft | kRight,
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

WallGrid::WallGrid(int size)
    : occupied_nodes_(), tmp_occupied_nodes_(), size_(size)
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

bool WallGrid::add_tmp_wall(const Wall &wall) const
{
    bool rc = true;

    int wall_row = wall.node().row();
    int wall_col = wall.node().col();

    if ((wall_row < 0) || (wall_row >= size_)
            || (wall_col< 0) || (wall_col >= size_)) {
        return false;
    }
    else if ((wall.orientation() == WallEnumClass::WallOrientation::WO_Horizontal)
            && ((wall_row == 0)
                || (wall_row == size_ - 1)
                || (wall_col + wall.len() >= size_))) {
        return false;
    }
    else if ((wall.orientation() == WallEnumClass::WallOrientation::WO_Vertical)
            && ((wall_col == 0)
                || (wall_col == size_ - 1)
                || (wall_row + wall.len() >= size_))) {
        return false;
    }

    Node inc_node(0, 0);
    const NodeOccupiedSide *occ_sides;

    switch (wall.orientation()) {
    case WallEnumClass::WallOrientation::WO_Horizontal:
        inc_node.set_col(1);
        occ_sides = horiz_occ_sides_;
        break;
    case WallEnumClass::WallOrientation::WO_Vertical:
        inc_node.set_row(1);
        occ_sides = vert_occ_sides_;
        break;
    case WallEnumClass::WallOrientation::WO_Invalid:
    default:
        return false;
    }

    tmp_occupied_nodes_.clear();

    Node node(wall_row, wall_col);
    NodeOccupiedSide occ_side;
    for (int i = 0; i <= wall.len(); ++i) {
        if (i == 0) {
            occ_side = occ_sides[0];
        }
        else if (i < wall.len()) {
            occ_side = occ_sides[1];
        }
        else {
            occ_side = occ_sides[2];
        }

        if ((occupied_nodes_.count(node) == 0)
                || ((occupied_nodes_.at(node) & occ_side) == 0)) {
            tmp_occupied_nodes_[node] = occ_side;
        }
        // new wall intersects with earlier added wall
        else {
            rc = false;
            break;
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

void WallGrid::possible_walls(std::vector<Wall> *walls) const
{
    for (int i = 0; i < size_; ++i) {
        for (int j = 0; j < size_; ++j) {
            Node node(i, j);

            Wall hwall(WallEnumClass::WallOrientation::WO_Horizontal, node, 2);
            if (add_tmp_wall(hwall) == 0) {
                walls->push_back(hwall);
            }

            Wall vwall(WallEnumClass::WallOrientation::WO_Vertical, node, 2);
            if (add_tmp_wall(vwall) == 0) {
                walls->push_back(vwall);
            }
        }
    }
}
