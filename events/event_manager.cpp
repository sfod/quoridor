#include "event_manager.hpp"

static EventManager *g_event_mgr;

EventManager::EventManager()
{
    g_event_mgr = this;
}

EventManager::~EventManager()
{
}

bs2::connection EventManager::add_listener(const slot_t &listener,
        const EventType &event_type)
{
    return signal_list_[event_type].connect(listener);
}

bool EventManager::trigger_event(const std::shared_ptr<EventDataBase> &event) const
{
    bool processed = false;
    if (signal_list_.count(event->event_type()) != 0) {
        signal_list_.at(event->event_type())(event);
        processed = true;
    }
    return processed;
}

bool EventManager::queue_event(const std::shared_ptr<EventDataBase> &event)
{
    bool queued = false;
    if (signal_list_.count(event->event_type()) != 0) {
        event_list_.push_back(event);
        queued = true;
    }
    return queued;
}

bool EventManager::abort_event(const std::shared_ptr<EventDataBase> &event)
{
    bool success = false;
    auto ev = std::find(std::begin(event_list_), std::end(event_list_), event);
    if (ev != std::end(event_list_)) {
        event_list_.erase(ev);
        success = true;
    }
    return success;
}

bool EventManager::update()
{
    while (!event_list_.empty()) {
        std::shared_ptr<EventDataBase> event = event_list_.front();
        event_list_.pop_front();
        if (signal_list_.count(event->event_type()) != 0) {
            signal_list_.at(event->event_type())(event);
        }
    }
    return true;
}

EventManager *EventManager::get()
{
    BOOST_ASSERT(g_event_mgr != NULL);
    return g_event_mgr;
}
