#ifndef CEGUI_EXT_DRAGGED_WINDOW_HPP_
#define CEGUI_EXT_DRAGGED_WINDOW_HPP_

#include "CEGUI/CEGUI.h"

namespace CEGUI_Ext {

class DraggedWindow : public CEGUI::DefaultWindow {
public:
    DraggedWindow(const CEGUI::String &type, const CEGUI::String &name);
    virtual ~DraggedWindow();

private:
    bool handle_start_drag_(const CEGUI::EventArgs &e);
    bool handle_stop_drag_(const CEGUI::EventArgs &e);
    bool handle_move_(const CEGUI::EventArgs &e);

private:
    CEGUI::Event::Connection conn_;
    CEGUI::Vector2f mouse_pos_in_win_;
    bool is_dragged_;
};

}  // namespace CEGUI

#endif  // CEGUI_EXT_DRAGGED_WINDOW_HPP_
