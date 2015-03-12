#include "qt_view.hpp"

QtView::QtView(QObject *qparent) : QObject(qparent)
{
}

bool QtView::connect_button(const char *name, const char *slot, QObject **obj)
{
    QObject *qbutton = find_object_by_name(name);
    if (qbutton == NULL) {
        return false;
    }
    QObject::connect(
            qbutton, SIGNAL(clicked()),
            this, slot);
    if (obj != NULL) {
        *obj = qbutton;
    }
    return true;
}
