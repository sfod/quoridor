#include "player_handler.hpp"

PlayerHandler::PlayerHandler() : player_list_(), current_player_(-1)
{
}

bool PlayerHandler::add_player(ActorId actor_id)
{
    player_list_.push_back(actor_id);
    return true;
}

bool PlayerHandler::remove_player(ActorId actor_id)
{
    for (auto id : player_list_) {
        if (id == actor_id) {
            player_list_.remove(id);
            return true;
        }
    }
    return false;
}

void PlayerHandler::clear()
{
    player_list_.clear();
    current_player_ = -1;
}

ActorId PlayerHandler::next_player()
{
    auto it = player_list_.begin();
    auto it_end = player_list_.end();

    bool found = false;
    for (; it != it_end; ++it) {
        if ((*it) == current_player_) {
            found = true;
        }
        else if (found) {
            current_player_ = *it;
            break;
        }
    }

    // current player is not in player_list or at the end of player list,
    // either way return the 1st player
    if (it == it_end) {
        current_player_ = *player_list_.begin();
    }

    return current_player_;
}
