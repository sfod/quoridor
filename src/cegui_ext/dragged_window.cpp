#include "dragged_window.hpp"

namespace CEGUI_Ext {

using namespace CEGUI;

DraggedWindow::DraggedWindow(const String &type, const String &name)
    : DefaultWindow(type, name), conn_(), mouse_pos_in_win_(),
    is_dragged_(false)
{
    subscribeEvent(
            Window::EventMouseButtonDown,
            Event::Subscriber(&DraggedWindow::handle_start_drag_, this));
    subscribeEvent(
            Window::EventMouseButtonUp,
            Event::Subscriber(&DraggedWindow::handle_stop_drag_, this));
    subscribeEvent(
            Window::EventMouseLeavesArea,
            Event::Subscriber(&DraggedWindow::handle_stop_drag_, this));
}

DraggedWindow::~DraggedWindow()
{
}

bool DraggedWindow::handle_start_drag_(const EventArgs &e)
{
    auto me = static_cast<const MouseEventArgs&>(e);
    conn_ = me.window->subscribeEvent(
            Window::EventMouseMove,
            Event::Subscriber(&DraggedWindow::handle_move_, this));
    mouse_pos_in_win_ = CoordConverter::screenToWindow(
            *me.window,
            System::getSingleton().getDefaultGUIContext().
                    getMouseCursor().getPosition());
    is_dragged_ = true;
    return true;
}

bool DraggedWindow::handle_stop_drag_(const EventArgs &e)
{
    if (is_dragged_) {
        auto me = static_cast<const MouseEventArgs&>(e);
        conn_->disconnect();
        mouse_pos_in_win_ = CoordConverter::screenToWindow(
                *me.window,
                System::getSingleton().getDefaultGUIContext().
                        getMouseCursor().getPosition());
        is_dragged_ = false;

        WindowEventArgs we(me.window);
        // @todo add mouse position to the fired event
        me.window->getParent()->fireEvent(CEGUI::Window::EventDragDropItemDropped, we, "");
    }
    return true;
}

bool DraggedWindow::handle_move_(const EventArgs &e)
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
