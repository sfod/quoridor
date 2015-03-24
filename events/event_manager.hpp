#pragma once

#include <memory>
#include <boost/signals2.hpp>
#include "event_data.hpp"

namespace bs2 = boost::signals2;

class EventManager {
    typedef bs2::signal<void (const std::shared_ptr<EventData>&)> signal_t;
    typedef signal_t::slot_type slot_t;

public:
    EventManager();
    virtual ~EventManager();

    bs2::connection add_listener(const slot_t &listener,
            const EventType &event_type);

    bool trigger_event(const std::shared_ptr<EventData> &event) const;
    bool queue_event(const std::shared_ptr<EventData> &event);
    bool abort_event(const std::shared_ptr<EventData> &event);

    bool update();

    static EventManager *get();

private:
    std::map<EventType, signal_t> signal_list_;
    std::list<std::shared_ptr<EventData>> event_list_;
};
