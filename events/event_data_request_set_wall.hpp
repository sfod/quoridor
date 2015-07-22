#pragma once

#include "event_data.hpp"

class EventData_RequestSetWall : public EventData<EventData_RequestSetWall> {
public:
    EventData_RequestSetWall(ActorId actor_id, const Wall &wall);
    ActorId actor_id() const;
    const Wall &wall() const;

private:
    ActorId actor_id_;
    Wall wall_;
};
