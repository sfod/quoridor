#include "event_manager.hpp"

static EventManager *g_event_mgr;

EventManager::EventManager()
{
    g_event_mgr = this;
}

void EventManager::add_listener(const QObject *receiver,
        std::function<void(const std::shared_ptr<EventData>&)> slot,
        const EventType &event_type)
{
    if (signal_list_.count(event_type) == 0) {
        signal_list_[event_type] = std::make_shared<EventSignal>();
    }

    QObject::connect(signal_list_[event_type].get(), &EventSignal::eventFired, receiver, slot);
}

bool EventManager::trigger_event(const std::shared_ptr<EventData> &event) const
{
    bool processed = false;
    if (signal_list_.count(event->event_type()) != 0) {
        signal_list_.at(event->event_type())->eventFired(event);
        processed = true;
    }
    return processed;
}

bool EventManager::queue_event(const std::shared_ptr<EventData> &event)
{
    bool queued = false;
    if (signal_list_.count(event->event_type()) != 0) {
        event_list_.push_back(event);
        queued = true;
    }
    return queued;
}

bool EventManager::abort_event(const std::shared_ptr<EventData> &event)
{
    bool success = false;
    auto ev = std::find(std::begin(event_list_), std::end(event_list_), event);
    if (ev != std::end(event_list_)) {
        event_list_.erase(ev);
        success = true;
    }
    return success;
}

void EventManager::update()
{
    while (!event_list_.empty()) {
        std::shared_ptr<EventData> event = event_list_.front();
        event_list_.pop_front();
        if (signal_list_.count(event->event_type()) != 0) {
            signal_list_.at(event->event_type())->eventFired(event);
        }
    }
}

EventManager *EventManager::get()
{
    Q_ASSERT(g_event_mgr != nullptr);
    return g_event_mgr;
}
