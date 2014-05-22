#ifndef CEGUI_EXT_DRAGGED_WINDOW_HPP_
#define CEGUI_EXT_DRAGGED_WINDOW_HPP_

#include "CEGUI/CEGUI.h"

namespace CEGUI_Ext {

using namespace CEGUI;

class DraggedWindow : public DefaultWindow {
public:
    DraggedWindow(const String &type, const String &name);
    virtual ~DraggedWindow();

private:
    bool handle_start_drag_(const EventArgs &e);
    bool handle_stop_drag_(const EventArgs &e);
    bool handle_move_(const EventArgs &e);

private:
    Event::Connection conn_;
    Vector2f mouse_pos_in_win_;
    bool is_dragged_;
};

}  // namespace CEGUI

#endif  // CEGUI_EXT_DRAGGED_WINDOW_HPP_
