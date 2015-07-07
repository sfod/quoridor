#pragma once

#include <list>
#include "actors/actor.hpp"

class PlayerHandler
{
public:
    PlayerHandler();
    virtual ~PlayerHandler();

    bool add_player(ActorId actor_id);
    bool remove_player(ActorId actor_id);
    void clear();
    ActorId current_player() const { return current_player_; }
    ActorId next_player();

private:
    std::list<ActorId> player_list_;
    ActorId current_player_;
};
