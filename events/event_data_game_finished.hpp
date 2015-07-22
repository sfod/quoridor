#pragma once

#include "event_data.hpp"

class EventData_GameFinished : public EventData<EventData_GameFinished> {
public:
    explicit EventData_GameFinished(ActorId actor_id);
    ActorId actor_id() const;

private:
    ActorId actor_id_;
};
