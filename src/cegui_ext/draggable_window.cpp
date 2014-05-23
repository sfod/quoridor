#include "draggable_window.hpp"

namespace CEGUI_Ext {

using namespace CEGUI;

const String DraggableWindow::EventDraggableWindowStartDragging("DraggableWindowStartDragging");
const String DraggableWindow::EventDraggableWindowDropped("DraggableWindowDropped");

DraggableWindow::DraggableWindow(const String &type, const String &name)
    : DefaultWindow(type, name), conn_(), mouse_pos_in_win_(),
    is_dragged_(false), is_enabled_(true)
{
    subscribeEvent(
            Window::EventMouseButtonDown,
            Event::Subscriber(&DraggableWindow::handle_start_drag_, this));
    subscribeEvent(
            Window::EventMouseButtonUp,
            Event::Subscriber(&DraggableWindow::handle_stop_drag_, this));
    subscribeEvent(
            Window::EventMouseLeavesArea,
            Event::Subscriber(&DraggableWindow::handle_stop_drag_, this));
}

DraggableWindow::~DraggableWindow()
{
}

void DraggableWindow::disable_drag()
{
    is_enabled_ = false;
}

void DraggableWindow::enable_drag()
{
    is_enabled_ = true;
}

bool DraggableWindow::handle_start_drag_(const EventArgs &e)
{
    if (!is_enabled_) {
        return false;
    }

    auto me = static_cast<const MouseEventArgs&>(e);
    conn_ = me.window->subscribeEvent(
            Window::EventMouseMove,
            Event::Subscriber(&DraggableWindow::handle_move_, this));
    mouse_pos_in_win_ = CoordConverter::screenToWindow(
            *me.window,
            System::getSingleton().getDefaultGUIContext().
                    getMouseCursor().getPosition());
    UVector2 mouse_offset_in_win(
            {0, mouse_pos_in_win_.d_x},
            {0, mouse_pos_in_win_.d_y});
    is_dragged_ = true;

    DragEvent de(me.window, mouse_offset_in_win);
    me.window->getParent()->fireEvent(EventDraggableWindowStartDragging, de, "");

    return true;
}

bool DraggableWindow::handle_stop_drag_(const EventArgs &e)
{
    if (is_dragged_) {
        auto me = static_cast<const MouseEventArgs&>(e);
        conn_->disconnect();
        mouse_pos_in_win_ = CoordConverter::screenToWindow(
                *me.window,
                System::getSingleton().getDefaultGUIContext().
                        getMouseCursor().getPosition());
        UVector2 mouse_offset_in_win(
                {0, mouse_pos_in_win_.d_x},
                {0, mouse_pos_in_win_.d_y});
        is_dragged_ = false;

        DragEvent de(me.window, mouse_offset_in_win);
        me.window->getParent()->fireEvent(EventDraggableWindowDropped, de, "");
    }
    return true;
}

bool DraggableWindow::handle_move_(const EventArgs &e)
{
    auto me = static_cast<const MouseEventArgs&>(e);
    Vector2f local_pos = CoordConverter::screenToWindow(*me.window, me.position);

    UVector2 mouse_move_offset({0, local_pos.d_x}, {0, local_pos.d_y});
    UVector2 mouse_offset_in_win(
            {0, mouse_pos_in_win_.d_x},
            {0, mouse_pos_in_win_.d_y});

    me.window->setPosition(me.window->getPosition() + mouse_move_offset
            - mouse_offset_in_win);
    return true;
}

}  // namespace CEGUI
