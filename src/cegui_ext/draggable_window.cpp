#include "draggable_window.hpp"
#include "logger.hpp"


static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

namespace CEGUI_Ext {

using namespace CEGUI;

const String DraggableWindow::EventDraggableWindowStartDragging("DraggableWindowStartDragging");
const String DraggableWindow::EventDraggableWindowDropped("DraggableWindowDropped");

DraggableWindow::DraggableWindow(const String &type, const String &name)
    : Window(type, name), mouse_pos_in_win_(), is_dragging_(false),
    is_enabled_(true)
{
    lg.add_attribute("Tag", blattrs::constant<std::string>("draggable window"));
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

void DraggableWindow::onMouseButtonDown(MouseEventArgs &me)
{
    if (!is_enabled_) {
        return;
    }

    activate();
    if (captureInput()) {
        mouse_pos_in_win_ = CoordConverter::screenToWindow(
                *me.window,
                System::getSingleton().getDefaultGUIContext().
                        getMouseCursor().getPosition());
        UVector2 mouse_offset_in_win(
                {0, mouse_pos_in_win_.d_x},
                {0, mouse_pos_in_win_.d_y});
        is_dragging_ = true;

        notifyScreenAreaChanged();

        DragEvent de(me.window, mouse_offset_in_win);
        me.window->getParent()->fireEvent(EventDraggableWindowStartDragging, de, "");
    }
    else {
        BOOST_LOG_SEV(lg, boost::log::trivial::warning)
            << "failed to capture input";
    }
}

void DraggableWindow::onMouseButtonUp(MouseEventArgs &me)
{
    if (is_dragging_) {
        mouse_pos_in_win_ = CoordConverter::screenToWindow(
                *me.window,
                System::getSingleton().getDefaultGUIContext().
                        getMouseCursor().getPosition());
        UVector2 mouse_offset_in_win(
                {0, mouse_pos_in_win_.d_x},
                {0, mouse_pos_in_win_.d_y});
        is_dragging_ = false;

        DragEvent de(me.window, mouse_offset_in_win);
        me.window->getParent()->fireEvent(EventDraggableWindowDropped, de, "");

        releaseInput();
    }
}

void DraggableWindow::onMouseMove(MouseEventArgs &me)
{
    if (is_dragging_) {
        Vector2f local_pos = CoordConverter::screenToWindow(*me.window, me.position);

        UVector2 mouse_move_offset({0, local_pos.d_x}, {0, local_pos.d_y});
        UVector2 mouse_offset_in_win(
                {0, mouse_pos_in_win_.d_x},
                {0, mouse_pos_in_win_.d_y});

        me.window->setPosition(me.window->getPosition() + mouse_move_offset
                - mouse_offset_in_win);
    }
}

void DraggableWindow::onMouseLeavesArea(CEGUI::MouseEventArgs &me)
{
    onMouseButtonUp(me);
}

void DraggableWindow::onCaptureLost(CEGUI::WindowEventArgs &we)
{
    Window::onCaptureLost(we);
}

}  // namespace CEGUI
