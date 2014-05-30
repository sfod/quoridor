#ifndef CEGUI_EXT_DRAG_EVENT_HPP_
#define CEGUI_EXT_DRAG_EVENT_HPP_

#include "CEGUI/CEGUI.h"

namespace CEGUI_Ext {

class DragEvent : public CEGUI::EventArgs {
public:
    DragEvent(CEGUI::Window *win, CEGUI::UVector2 pos);
    virtual ~DragEvent();

    CEGUI::Window *window() const;
    CEGUI::UVector2 pos() const;

private:
    CEGUI::Window *win_;
    CEGUI::UVector2 pos_;
};

}  // namespace CEGUI_Ext

#endif  // CEGUI_EXT_DRAG_EVENT_HPP_
