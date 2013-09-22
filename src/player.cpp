#include "player.hpp"

#include <iostream>

namespace Quoridor {

Player::Player(const std::string &name, Board *board)
    : name_(name), board_(board)
{
}

Player::~Player()
{
}

int Player::make_move()
{
    int rc = 0;
    int move;

    std::cout << "make move ";
    std::cin >> move;
    switch (move) {
    case 1:
        rc = move_forward();
        break;
    case 2:
        rc = move_back();
        break;
    case 3:
        rc = move_right();
        break;
    case 4:
        rc = move_left();
        break;
    default:
        std::cerr << "invalid move" << std::endl;
        rc = -1;
        break;
    }

    return rc;
}

int Player::move_forward()
{
    std::cout << "moving forward" << std::endl;
    return 0;
}

int Player::move_back()
{
    std::cout << "moving back" << std::endl;
    return 0;
}

int Player::move_right()
{
    std::cout << "moving right" << std::endl;
    return 0;
}

int Player::move_left()
{
    std::cout << "moving left" << std::endl;
    return 0;
}

}  /* namespace Quoridor */
