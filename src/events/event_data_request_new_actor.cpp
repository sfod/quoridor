#include "event_data_request_new_actor.hpp"
#include <functional>

template<> EventType EventData_RequestNewActor::Base::event_type_ = std::hash<std::string>()("RequestNewActor");

EventData_RequestNewActor::EventData_RequestNewActor(PlayerType ptype) : ptype_(ptype)
{
}

PlayerType EventData_RequestNewActor::player_type() const
{
    return ptype_;
}
