#include "event_data_game_finished.hpp"
#include <functional>

template<> EventType EventData_GameFinished::Base::event_type_ = std::hash<std::string>()("GameFinished");

EventData_GameFinished::EventData_GameFinished(ActorId actor_id) : actor_id_(actor_id)
{
}

ActorId EventData_GameFinished::actor_id() const
{
    return actor_id_;
}
