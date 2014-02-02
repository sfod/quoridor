#include "ui.hpp"

namespace Quoridor {
namespace UI {

UI::UI()
{
}

UI::~UI()
{
}

void UI::draw_window()
{
    ui_impl_->draw_window();
}

void UI::update(const std::vector<std::vector<char>> &repr)
{
    ui_impl_->update(repr);
}

bool UI::poll_event(Event *ev)
{
    return ui_impl_->poll_event(ev);
}

}  /* namespace UI */
}  /* namespace Quoridor */
