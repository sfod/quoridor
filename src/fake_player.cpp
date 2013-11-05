#include "fake_player.hpp"
#include "walk_move.hpp"

namespace Quoridor {

FakePlayer::FakePlayer()
{
}

FakePlayer::~FakePlayer()
{
}

IMove *FakePlayer::get_move()
{
    return new WalkMove(WalkMove::Direction::kForward);
}

}  /* namespace Quoridor */
