#ifndef QUORIDOR_WALL_HPP_
#define QUORIDOR_WALL_HPP_

#include <iostream>

namespace Quoridor {

class Wall {
public:
    enum Orientation {
        kHorizontal,
        kVertical,
        kInvalid
    };

public:
    Wall();
    Wall(Orientation orientation, int row, int col, int cnt);
    virtual ~Wall();

    void set_orientation(Orientation orientation) { orientation_ = orientation; }
    void set_row(int row) { row_ = row; }
    void set_col(int col) { col_ = col; }
    void set_cnt(int cnt) { cnt_ = cnt; }

    Orientation orientation() const { return orientation_; }
    int row() const { return row_; }
    int col() const { return col_; }
    int cnt() const { return cnt_; }

private:
    Orientation orientation_;
    int row_;
    int col_;
    int cnt_;
};

std::ostream &operator<<(std::ostream &os, const Quoridor::Wall &wall);

}  /* namespace Quoridor */

#endif  /* QUORIDOR_WALL_HPP_ */
