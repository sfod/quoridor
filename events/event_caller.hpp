#pragma once

#include <QObject>

class EventCaller : public QObject {
    Q_OBJECT
public:
    explicit EventCaller(QObject *parent = 0);

signals:

public slots:
    bool update() const;
};
