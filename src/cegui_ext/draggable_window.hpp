#ifndef CEGUI_EXT_DRAGGABLE_WINDOW_HPP_
#define CEGUI_EXT_DRAGGABLE_WINDOW_HPP_

#include "CEGUI/CEGUI.h"
#include "drag_event.hpp"

namespace CEGUI_Ext {

class DraggableWindow : public CEGUI::Window {
public:
    DraggableWindow(const CEGUI::String &type, const CEGUI::String &name);
    virtual ~DraggableWindow();

    void disable_drag();
    void enable_drag();

protected:
    virtual void onMouseButtonDown(CEGUI::MouseEventArgs &me);
    virtual void onMouseButtonUp(CEGUI::MouseEventArgs &me);
    virtual void onMouseMove(CEGUI::MouseEventArgs &me);
    virtual void onMouseLeavesArea(CEGUI::MouseEventArgs &me);
    virtual void onCaptureLost(CEGUI::WindowEventArgs &we);

public:
    static const CEGUI::String EventDraggableWindowStartDragging;
    static const CEGUI::String EventDraggableWindowDropped;

private:
    bool handle_start_drag_(const CEGUI::EventArgs &e);
    bool handle_stop_drag_(const CEGUI::EventArgs &e);
    bool handle_move_(const CEGUI::EventArgs &e);

private:
    CEGUI::Vector2f mouse_pos_in_win_;
    bool is_dragging_;
    bool is_enabled_;
};

}  // namespace CEGUI

#endif  // CEGUI_EXT_DRAGGABLE_WINDOW_HPP_
