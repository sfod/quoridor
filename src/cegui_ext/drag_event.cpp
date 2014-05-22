#include "drag_event.hpp"

namespace CEGUI_Ext {

using namespace CEGUI;

DragEvent::DragEvent(Window *win, UVector2 pos)
    : EventArgs(), win_(win), pos_(pos)
{
}

DragEvent::~DragEvent()
{
}

Window *DragEvent::window() const
{
    return win_;
}

UVector2 DragEvent::pos() const
{
    return pos_;
}

}  // namespace CEGUI_Ext
