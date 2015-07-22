#pragma once

#include <QObject>
#include "iview.hpp"

class QtView : public QObject, public IView {
    Q_OBJECT

public:
    explicit QtView(QObject *qparent = nullptr);

    virtual void on_msg() override = 0;
    virtual void on_update() override = 0;
    virtual void attach(ActorId actor_id) override = 0;

protected:
    virtual QObject *find_object_by_name(const char *name) const = 0;
    virtual void connect_button(const char *name, const char *slot, QObject **obj = NULL);
};
