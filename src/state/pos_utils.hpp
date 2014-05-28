#ifndef QUORIDOR_POS_UTILS_HPP_
#define QUORIDOR_POS_UTILS_HPP_

#include <CEGUI/CEGUI.h>
#include "node.hpp"

namespace Quoridor {

class PosUtils {
public:
    PosUtils(int node_cnt, int node_width, int border_width);
    ~PosUtils();

public:
    Node pos_to_node(const CEGUI::UVector2 &pos) const;
    CEGUI::UVector2 node_to_pos(const Node &node) const;

private:
    int node_cnt_;
    int node_width_;
    int border_width_;
};

}  // namespace Quoridor

#endif  // QUORIDOR_POS_UTILS_HPP_
