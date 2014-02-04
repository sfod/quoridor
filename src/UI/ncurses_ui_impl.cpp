#include "ncurses_ui_impl.hpp"
#include "exception.hpp"

namespace Quoridor {
namespace UI {

NcursesUIImpl::NcursesUIImpl() : win_(), menu_()
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

NcursesUIImpl::~NcursesUIImpl()
{
    endwin();
}

void NcursesUIImpl::draw_window()
{
    int height = 21;
    int width = 21;

    win_ = newwin(height, width, 0, 0);
    box(win_, 0, 0);
    wrefresh(win_);
}

void NcursesUIImpl::update(const std::vector<std::vector<char>> &repr)
{
    int i = 1;
    wmove(win_, i, 1);
    for (auto cols : repr) {
        for (auto col : cols) {
            waddch(win_, col);
        }
        ++i;
        wmove(win_, i, 1);
    }
    wrefresh(win_);
}

void NcursesUIImpl::add_menu(const std::vector<std::string> &items)
{
    menu_ = std::shared_ptr<NcursesMenu>(new NcursesMenu(win_, items));
}

void NcursesUIImpl::up_menu()
{
    if (menu_) {
        menu_->up();
    }
    throw Exception("menu is not initialized");
}

void NcursesUIImpl::down_menu()
{
    if (menu_) {
        menu_->down();
    }
    throw Exception("menu is not initialized");
}

std::string NcursesUIImpl::menu_item()
{
    if (menu_) {
        return menu_->item();
    }
    throw Exception("menu is not initialized");
}

}  /* namespace UI */
}  /* namespace Quoridor */
