#ifndef QT_VIEW_HPP
#define QT_VIEW_HPP

#include <QObject>
#include "iview.hpp"

class QtView : public QObject, public IView {
    Q_OBJECT

public:
    explicit QtView(QObject *qparent = 0);
    virtual bool init() = 0;
    virtual void on_msg() = 0;
    virtual void on_update() = 0;
    virtual void attach(ActorId actor_id) = 0;

protected:
    virtual QObject *find_object_by_name(const char *name) const = 0;
    virtual bool connect_button(const char *name, const char *slot,
            QObject **obj = NULL);
};

#endif // QT_VIEW_HPP
