#ifndef EVENT_CALLER_HPP
#define EVENT_CALLER_HPP

#include <QObject>

class EventCaller : public QObject {
    Q_OBJECT
public:
    explicit EventCaller(QObject *parent = 0);

signals:

public slots:
    bool update() const;
};

#endif // EVENT_CALLER_HPP
