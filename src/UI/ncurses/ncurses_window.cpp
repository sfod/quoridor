#include "ncurses_window.hpp"

namespace Quoridor {
namespace UI {

NcursesWindow::NcursesWindow()
{
    int height = 21;
    int width = 21;
    win_ = newwin(height, width, 0, 0);
    box(win_, 0, 0);
    wrefresh(win_);
}

NcursesWindow::~NcursesWindow()
{
    delwin(win_);
}

void NcursesWindow::draw_board(const std::vector<std::vector<char>> &repr)
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
