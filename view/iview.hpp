#ifndef IVIEW_HPP
#define IVIEW_HPP

#include "actors/actor.hpp"

class IView
{
public:
    virtual ~IView() {}

    virtual bool init() = 0;
    virtual void on_msg() = 0;
    virtual void on_update() = 0;
    virtual void attach(ActorId actor_id) = 0;
};

#endif // IVIEW_HPP
