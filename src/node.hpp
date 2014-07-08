#ifndef QUORIDOR_POS_HPP_
#define QUORIDOR_POS_HPP_

#include <iostream>
#include <vector>

namespace Quoridor {

class Node {
public:
    Node();
    Node(int row, int col);
    Node(const Node &node);

    int row() const { return row_; }
    int col() const { return col_; }

    void set_row(int row) { row_ = row; }
    void set_col(int col) { col_ = col; }

    void neighbours(int row_num, int col_num,
            std::vector<Node> *neighbours) const;

    bool operator<(const Node &node) const;
    bool operator==(const Node &node) const;
    bool operator!=(const Node &node) const;
    const Node &operator+=(const Node &node);
    const Node operator+(const Node &node);

private:
    int row_;
    int col_;
};

std::ostream &operator<<(std::ostream &os, const Node &node);

}  // namespace Quoridor

#endif  // QUORIDOR_POS_HPP_
