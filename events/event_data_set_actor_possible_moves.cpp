#include "event_data_set_actor_possible_moves.hpp"
#include <functional>

template<> EventType EventData_SetActorPossibleMoves::Base::event_type_ = std::hash<std::string>()("SetActorPossibleMoves");

EventData_SetActorPossibleMoves::EventData_SetActorPossibleMoves(
        ActorId actor_id,
        const std::list<Node> &possible_moves)
    : actor_id_(actor_id), possible_moves_(possible_moves)
{
}

ActorId EventData_SetActorPossibleMoves::actor_id() const
{
    return actor_id_;
}

const std::list<Node> &EventData_SetActorPossibleMoves::possible_moves() const
{
    return possible_moves_;
}
