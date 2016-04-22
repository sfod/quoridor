#include "event_data_new_actor.hpp"
#include <functional>

template<> EventType EventData_NewActor::Base::event_type_ = std::hash<std::string>()("NewActor");

EventData_NewActor::EventData_NewActor(ActorId actor_id, const Node &node,
        const std::list<Node> &possible_moves, int wall_num)
    : actor_id_(actor_id), node_(node), possible_moves_(possible_moves), wall_num_(wall_num)
{
}

ActorId EventData_NewActor::actor_id() const
{
    return actor_id_;
}

Node EventData_NewActor::node() const
{
    return node_;
}

const std::list<Node> &EventData_NewActor::possible_moves() const
{
    return possible_moves_;
}

int EventData_NewActor::wall_num() const
{
    return wall_num_;
}
