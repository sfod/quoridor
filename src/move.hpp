#ifndef QUORIDOR_MOVE_HPP_
#define QUORIDOR_MOVE_HPP_

namespace Quoridor {

class Move {
public:
    Move();
    explicit Move(int action);
    ~Move();

    void set_action(int action);
    int action() const;

    void set_cont(bool cont);
    bool cont() const;

public:
    enum BoardMoves {
        kForward = 0,
        kRight = 1,
        kBackward = 2,
        kLeft = 3,
        kPutWall = 4,
        kCancel = 5,
        kEND,
    };

private:
    int action_;
    bool cont_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_MOVE_HPP_ */
