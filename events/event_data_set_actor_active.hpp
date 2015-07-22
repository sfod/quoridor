#pragma once

#include "event_data.hpp"

class EventData_SetActorActive : public EventDataCRTP<EventData_SetActorActive> {
public:
    explicit EventData_SetActorActive(ActorId actor_id);
    ActorId actor_id() const;

private:
    ActorId actor_id_;
};
