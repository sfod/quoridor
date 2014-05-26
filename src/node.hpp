#ifndef QUORIDOR_POS_HPP_
#define QUORIDOR_POS_HPP_

namespace Quoridor {

class Node {
public:
    Node();
    Node(int row, int col);
    Node(const Node &node);

    int row() const { return row_; }
    int col() const { return col_; }

    bool operator<(const Node &node) const;
    bool operator==(const Node &node) const;
    bool operator!=(const Node &node) const;
    const Node &operator+=(const Node &node);
    const Node operator+(const Node &node);

private:
    int row_;
    int col_;
};

}  // namespace Quoridor

#endif  // QUORIDOR_POS_HPP_
