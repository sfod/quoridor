#pragma once

#include "event_data.hpp"

class EventData_SetWall : public EventData<EventData_SetWall> {
public:
    EventData_SetWall(ActorId actor_id, const Wall &wall);
    ActorId actor_id() const;
    const Wall &wall() const;

private:
    ActorId actor_id_;
    Wall wall_;
};
