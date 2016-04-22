#pragma once

#include <memory>
#include <QObject>
#include "event_data.hpp"


class EventSignal : public QObject {
    Q_OBJECT

signals:
    void eventFired(const std::shared_ptr<EventData> &event);
};


class EventManager {
public:
    void add_listener(const QObject *receiver,
            std::function<void(const std::shared_ptr<EventData>&)> slot,
            const EventType &event_type);

    bool trigger_event(const std::shared_ptr<EventData> &event) const;
    bool queue_event(const std::shared_ptr<EventData> &event);
    bool abort_event(const std::shared_ptr<EventData> &event);

    void update();

private:
    std::map<EventType, std::shared_ptr<EventSignal>> signal_list_;
    std::list<std::shared_ptr<EventData>> event_list_;
};
