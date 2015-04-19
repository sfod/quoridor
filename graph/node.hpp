#pragma once

#include <QDebug>
#include <iostream>
#include <list>

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
            std::list<Node> *neighbours) const;

    bool operator<(const Node &node) const;
    bool operator==(const Node &node) const;
    bool operator!=(const Node &node) const;
    const Node &operator+=(const Node &node);
    const Node operator+(const Node &node);

    QString to_string() const;

private:
    int row_;
    int col_;
};

std::ostream &operator<<(std::ostream &os, const Node &node);
QDebug operator<<(QDebug dbg, const Node &node);
