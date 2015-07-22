#include "event_data_request_actor_move.hpp"
#include <functional>

template<> EventType EventData_RequestActorMove::Base::event_type_ = std::hash<std::string>()("RequestActorMove");

EventData_RequestActorMove::EventData_RequestActorMove(ActorId actor_id,
        const Node &node)
    : actor_id_(actor_id), node_(node)
{
}

ActorId EventData_RequestActorMove::actor_id() const
{
    return actor_id_;
}

Node EventData_RequestActorMove::node() const
{
    return node_;
}
