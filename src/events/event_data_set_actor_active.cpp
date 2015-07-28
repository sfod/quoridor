#include "event_data_set_actor_active.hpp"
#include <functional>

template<> EventType EventData_SetActorActive::Base::event_type_ = std::hash<std::string>()("SetActorActive");

EventData_SetActorActive::EventData_SetActorActive(ActorId actor_id)
    : actor_id_(actor_id)
{
}

ActorId EventData_SetActorActive::actor_id() const
{
    return actor_id_;
}
