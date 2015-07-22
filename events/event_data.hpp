#pragma once

#include "actors/actor.hpp"
#include "game/game_data.hpp"
#include "graph/node.hpp"
#include "graph/wall.hpp"

typedef std::size_t EventType;

/*
 * Base class for events
 */
class EventDataBase {
public:
    virtual ~EventDataBase() = default;
    virtual EventType event_type() const = 0;
};


template<typename Derived>
class EventData : public EventDataBase {
public:
    typedef EventData<Derived> Base;

    static EventType static_event_type() { return event_type_; }
    virtual EventType event_type() const override { return event_type_; }

protected:
    static EventType event_type_;
};
