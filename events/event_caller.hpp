#pragma once

#include <QObject>

class EventCaller : public QObject {
    Q_OBJECT
public:
    explicit EventCaller(QObject *parent = nullptr);

signals:

public slots:
    void update() const;
};
