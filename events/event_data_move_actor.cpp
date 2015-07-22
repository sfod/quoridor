#include "event_data_move_actor.hpp"
#include <functional>

template<> EventType EventData_MoveActor::Base::event_type_ = std::hash<std::string>()("MoveActor");

EventData_MoveActor::EventData_MoveActor(ActorId actor_id, const Node &node,
        const std::list<Node> &possible_moves)
    : actor_id_(actor_id), node_(node), possible_moves_(possible_moves)
{
}

ActorId EventData_MoveActor::actor_id() const
{
    return actor_id_;
}

Node EventData_MoveActor::node() const
{
    return node_;
}

const std::list<Node> &EventData_MoveActor::possible_moves() const
{
    return possible_moves_;
}
