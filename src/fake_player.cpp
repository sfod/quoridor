#include "fake_player.hpp"

namespace Quoridor {

FakePlayer::FakePlayer()
{
}

FakePlayer::~FakePlayer()
{
}

int FakePlayer::get_move(Move *move)
{
    move->set_action(0);
    return 0;
}

}  /* namespace Quoridor */
