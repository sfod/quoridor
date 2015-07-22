#pragma once

#include "event_data.hpp"

class EventData_NewActor : public EventData<EventData_NewActor> {
public:
    explicit EventData_NewActor(ActorId actor_id, const Node &node, const std::list<Node> &possible_moves, int wall_num);
    ActorId actor_id() const;
    Node node() const;
    const std::list<Node> &possible_moves() const;
    int wall_num() const;

private:
    ActorId actor_id_;
    Node node_;
    std::list<Node> possible_moves_;
    int wall_num_;
};

