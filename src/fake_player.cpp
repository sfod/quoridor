#include "fake_player.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "walk_move.hpp"
#include "wall_move.hpp"

namespace Quoridor {

FakePlayer::FakePlayer()
{
    time_t ts = time(NULL);
    srand(ts);
}

FakePlayer::~FakePlayer()
{
}

IMove *FakePlayer::get_move()
{
    if (rand() % 20 > 2) {
        if (rand() % 2 == 0) {
            return new WalkMove(WalkMove::Direction::kForward);
        }
        else {
            return new WalkMove(rand() % 4);
        }
    }
    else {
        return new WallMove(Wall(rand() % 2, rand() % 9, rand() % 8, 2));
    }
}

}  /* namespace Quoridor */
