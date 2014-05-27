#include "node.hpp"
#include <tuple>

namespace Quoridor {

Node::Node() : row_(0), col_(0)
{
}

Node::Node(int row, int col) : row_(row), col_(col)
{
}

Node::Node(const Node &node)
{
    row_ = node.row_;
    col_ = node.col_;
}

bool Node::operator<(const Node &node) const
{
    return std::tie(row_, col_) < std::tie(node.row_, node.col_);
}

bool Node::operator==(const Node &node) const
{
    return (row_ == node.row_) && (col_ == node.col_);
}

bool Node::operator!=(const Node &node) const
{
    return !(*this == node);
}

const Node &Node::operator+=(const Node &node)
{
    row_ += node.row_;
    col_ += node.col_;
    return *this;
}
const Node Node::operator+(const Node &node) {
    Node n = *this;
    n += node;
    return n;
}

}  // namespace Quoridor
