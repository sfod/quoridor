#include "event_caller.hpp"

EventCaller::EventCaller(std::shared_ptr<EventManager> event_manager)
    : QObject(NULL), event_manager_(event_manager)
{
}

void EventCaller::update() const
{
    event_manager_->update();
}
