#pragma once

#include <map>
#include <memory>
#include "actor.hpp"

class ActorKeeper {
public:
    ActorKeeper();
    ~ActorKeeper();

    void add_actor(std::shared_ptr<Actor> &actor);
    std::shared_ptr<Actor> actor(ActorId id) const;

private:
    std::map<ActorId, std::shared_ptr<Actor>> actor_id_list_;
};
