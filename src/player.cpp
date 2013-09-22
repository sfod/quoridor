#include "player.hpp"

#include <iostream>

namespace Quoridor {

Player::Player(const std::string &name) : name_(name), pos_()
{
}

Player::~Player()
{
}

void Player::set_pos(const pos_t &pos)
{
    pos_ = pos;
}

}  /* namespace Quoridor */
