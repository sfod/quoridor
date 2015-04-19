#include "node.hpp"
#include <tuple>

Node::Node() : row_(-1), col_(-1)
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

void Node::neighbours(int row_num, int col_num,
        std::list<Node> *neighbours) const
{
    int n[3][3] = {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}};
    if (row_ - 1 < 0) {
        n[0][0] = 0;
        n[0][1] = 0;
        n[0][2] = 0;
    }
    if (row_ + 1 >= row_num) {
        n[2][0] = 0;
        n[2][1] = 0;
        n[2][2] = 0;
    }
    if (col_ - 1 < 0) {
        n[0][0] = 0;
        n[1][0] = 0;
        n[2][0] = 0;
    }
    if (col_ + 1 >= col_num) {
        n[0][2] = 0;
        n[1][2] = 0;
        n[2][2] = 0;
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (n[i][j] == 1) {
                neighbours->push_back(Node(row_ + i - 1, col_ + j - 1));
            }
        }
    }
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

QString Node::to_string() const
{
    return QString::number(row_) + ":" + QString::number(col_);
}

std::ostream &operator<<(std::ostream &os, const Node &node)
{
    return os << node.row() << ":" << node.col();
}

QDebug operator<<(QDebug dbg, const Node &node)
{
    return dbg.nospace() << node.to_string();
}
