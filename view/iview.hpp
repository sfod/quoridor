#pragma once

#include "actors/actor.hpp"

class IView
{
public:
    virtual ~IView() = default;

    virtual void on_msg() = 0;
    virtual void on_update() = 0;
    virtual void attach(ActorId actor_id) = 0;
};
