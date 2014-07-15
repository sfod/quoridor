#include "mock_player.hpp"
#include "exception.hpp"

namespace Quoridor {

MockPlayer::MockPlayer() : moves_()
{
}

MockPlayer::~MockPlayer()
{
}

void MockPlayer::get_move(std::function<void(move_t)> callback)
{
    if (moves_.size() == 0) {
        throw Exception("no more moves");
    }
    move_t move = moves_.front();
    moves_.pop_front();
    callback(move);
}

void MockPlayer::push_move(move_t move)
{
    moves_.push_back(move);
}

}  /* namespace Quoridor */
