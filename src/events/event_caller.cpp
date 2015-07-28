#include "event_caller.hpp"
#include "event_manager.hpp"

EventCaller::EventCaller(QObject *parent) : QObject(parent)
{
}

void EventCaller::update() const
{
    EventManager::get()->update();
}
