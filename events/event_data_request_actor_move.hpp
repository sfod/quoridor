#pragma once

#include "event_data.hpp"

class EventData_RequestActorMove : public EventData<EventData_RequestActorMove> {
public:
    EventData_RequestActorMove(ActorId actor_id, const Node &node);
    ActorId actor_id() const;
    Node node() const;

private:
    ActorId actor_id_;
    Node node_;
};
