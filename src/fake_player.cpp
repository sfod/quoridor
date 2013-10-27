#include "fake_player.hpp"

namespace Quoridor {

FakePlayer::FakePlayer()
{
}

FakePlayer::~FakePlayer()
{
}

int FakePlayer::get_move(BoardMoves *move)
{
    *move = static_cast<BoardMoves>(0);
    return 0;
}

}  /* namespace Quoridor */
