#include "ncurses_ui_impl.hpp"

namespace Quoridor {
namespace UI {

NcursesUIImpl::NcursesUIImpl() : win_()
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

}  /* namespace UI */
}  /* namespace Quoridor */
