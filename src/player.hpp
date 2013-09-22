#pragma once

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

private:
    std::string name_;
    pos_t pos_;
};

}  /* namespace Quoridor */
