#include "wall.hpp"

Wall::Wall(WallEnumClass::WallOrientation wo, Node node, int len)
    : orientation_(wo), node_(node)
{
    Node node1;
    Node node2;
    if (wo == WallEnumClass::WallOrientation::WO_Horizontal) {
        for (int i = 0; i < len; ++i) {
            node1 = Node(node.row() - 1, node.col() + i);
            node2 = Node(node.row(), node.col() + i);
            affected_nodes_.push_back(std::make_pair(node1, node2));
        }
    }
    else if (wo == WallEnumClass::WallOrientation::WO_Vertical) {
        for (int i = 0; i < len; ++i) {
            node1 = Node(node.row() + i, node.col() - 1);
            node2 = Node(node.row() + i, node.col());
            affected_nodes_.push_back(std::make_pair(node1, node2));
        }
    }
}

Wall::~Wall()
{
}

