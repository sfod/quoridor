#include "event_data_set_wall.hpp"
#include <functional>

template<> EventType EventData_SetWall::Base::event_type_ = std::hash<std::string>()("SetWall");

EventData_SetWall::EventData_SetWall(ActorId actor_id, const Wall &wall)
    : actor_id_(actor_id), wall_(wall)
{
}

ActorId EventData_SetWall::actor_id() const
{
    return actor_id_;
}

const Wall &EventData_SetWall::wall() const
{
    return wall_;
}
