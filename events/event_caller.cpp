#include "event_caller.hpp"
#include "event_manager.hpp"

EventCaller::EventCaller(QObject *parent) :
    QObject(parent)
{
}

bool EventCaller::update() const
{
    return EventManager::get()->update();
}
