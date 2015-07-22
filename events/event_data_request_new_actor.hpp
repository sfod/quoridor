#pragma once

#include "event_data.hpp"

class EventData_RequestNewActor : public EventDataCRTP<EventData_RequestNewActor> {
public:
    explicit EventData_RequestNewActor(PlayerType ptype);
    PlayerType player_type() const;

private:
    PlayerType ptype_;
};
