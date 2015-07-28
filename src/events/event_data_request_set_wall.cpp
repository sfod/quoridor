#include "event_data_request_set_wall.hpp"
#include <functional>

template<> EventType EventData_RequestSetWall::Base::event_type_ = std::hash<std::string>()("RequestSetWall");

EventData_RequestSetWall::EventData_RequestSetWall(ActorId actor_id, const Wall &wall)
    : actor_id_(actor_id), wall_(wall)
{
}

ActorId EventData_RequestSetWall::actor_id() const
{
    return actor_id_;
}

const Wall &EventData_RequestSetWall::wall() const
{
    return wall_;
}
