#include "player.hpp"
#include <iostream>

namespace Quoridor {

Player::Player()
{
}

Player::~Player()
{
}

int Player::get_move(Move *move)
{
    int i;
    std::cin >> i;

    move->set_action(i);

    return 0;
}

}  /* namespace Quoridor */
