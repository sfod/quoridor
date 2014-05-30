#ifndef QUORIDOR_WALL_HPP_
#define QUORIDOR_WALL_HPP_

namespace Quoridor {

class Wall {
public:
    Wall();
    Wall(int orientation, int line, int start_pos, int cnt);
    virtual ~Wall();

    void set_orientation(int orientation) { orientation_ = orientation; }
    void set_line(int line) { line_ = line; }
    void set_start_pos(int start_pos) { start_pos_ = start_pos; }
    void set_cnt(int cnt) { cnt_ = cnt; }

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
