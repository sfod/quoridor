#pragma once

#include <QObject>
#include "events/event_manager.hpp"

class EventCaller : public QObject {
    Q_OBJECT
public:
    explicit EventCaller(std::shared_ptr<EventManager> event_manager);

signals:

public slots:
    void update() const;

private:
    std::shared_ptr<EventManager> event_manager_;
};
