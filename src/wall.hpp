#ifndef QUORIDOR_WALL_HPP_
#define QUORIDOR_WALL_HPP_

namespace Quoridor {

class Wall {
public:
    Wall(int orientation, int line, int start_pos, int cnt);
    virtual ~Wall();

    int orientation() const { return orientation_; }
    int line() const { return line_; }
    int start_pos() const { return start_pos_; }
    int end_pos() const { return start_pos_ + cnt_ - 1; }
    int cnt() const { return cnt_; }

private:
    int orientation_;
    int line_;
    int start_pos_;
    int cnt_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_WALL_HPP_ */
