#include "pos_utils.hpp"

namespace Quoridor {

PosUtils::PosUtils(int node_cnt, int node_width, int border_width)
    : node_cnt_(node_cnt), node_width_(node_width), border_width_(border_width)
{
}

PosUtils::~PosUtils()
{
}

Node PosUtils::pos_to_node(const CEGUI::UVector2 &pos) const
{
    static int total_width = node_width_ * node_cnt_ + 2 * border_width_;

    Node n;
    n.set_row(static_cast<int>((pos.d_y.d_scale * total_width - border_width_) / node_width_));
    n.set_col(static_cast<int>((pos.d_x.d_scale * total_width - border_width_) / node_width_));

    // normalize node position
    if (n.row() >= node_cnt_) {
        n.set_row(node_cnt_ - 1);
    }
    else if (n.row() < 0) {
        n.set_row(0);
    }
    if (n.col() >= node_cnt_) {
        n.set_col(node_cnt_ - 1);
    }
    else if (n.col() < 0) {
        n.set_col(0);
    }

    // revert rows
    n.set_row(node_cnt_ - 1 - n.row());

    return n;
}

CEGUI::UVector2 PosUtils::node_to_pos(const Node &node) const
{
    static int total_width = node_width_ * node_cnt_ + 2 * border_width_;

    CEGUI::UVector2 p({0.0, 0.0}, {0.0, 0.0});
    p.d_y.d_scale = static_cast<float>(border_width_ + ((node_cnt_ - 1) - node.row()) * node_width_) / total_width;
    p.d_x.d_scale = static_cast<float>(border_width_ + node.col() * node_width_) / total_width;
    return p;
}

}  // namespace Quoridor
