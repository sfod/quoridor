#include "qt_view.hpp"
#include "exceptions/exception.hpp"

QtView::QtView(QObject *qparent) : QObject(qparent)
{
}

void QtView::connect_button(const char *name, const char *slot, QObject **obj)
{
    QObject *qbutton = find_object_by_name(name);
    if (!qbutton) {
        throw qml_missing_element_error();
    }

    if (!QObject::connect(qbutton, SIGNAL(clicked()), this, slot)) {
        throw qml_connect_error();
    }

    if (obj != NULL) {
        *obj = qbutton;
    }
}
