#pragma once

#include "event_data.hpp"

class EventData_MoveActor : public EventDataCRTP<EventData_MoveActor> {
public:
    EventData_MoveActor(ActorId actor_id, const Node &node, const std::list<Node> &possible_moves);
    ActorId actor_id() const;
    Node node() const;
    const std::list<Node> &possible_moves() const;

private:
    ActorId actor_id_;
    Node node_;
    std::list<Node> possible_moves_;
};
