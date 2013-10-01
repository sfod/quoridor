#ifndef QUORIDOR_PLAYER_HPP_
#define QUORIDOR_PLAYER_HPP_

#include <string>
#include "board.hpp"

namespace Quoridor {

class Player {
public:
    explicit Player(const std::string &name);
    virtual ~Player();

    const std::string& name() const { return name_; }

    pos_t pos() const { return pos_; }
    void set_pos(const pos_t &pos);

    int board_side() const { return board_side_; }
    void set_board_side(int board_side) { board_side_ = board_side; }

private:
    std::string name_;
    int board_side_;
    pos_t pos_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_PLAYER_HPP_ */
