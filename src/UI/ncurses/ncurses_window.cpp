#include "ncurses_window.hpp"
#include "ncurses_menu.hpp"

#include <menu.h>

namespace Quoridor {
namespace UI {

NcursesWindow::NcursesWindow()
{
    int height = 21;
    int width = 21;
    win_ = newwin(height, width, 0, 0);
    box(win_, 0, 0);
    refresh();
}

NcursesWindow::~NcursesWindow()
{
    delwin(win_);
}

void NcursesWindow::draw()
{
    wrefresh(win_);
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
}

void NcursesWindow::add_menu(std::shared_ptr<Menu> menu)
{
    Menu *m = menu.get();
    NcursesMenu *nm = dynamic_cast<NcursesMenu*>(m);
    set_menu_win(nm->menu(), win_);
    set_menu_sub(nm->menu(), derwin(win_, 19, 19, 1, 1));
    post_menu(nm->menu());
}

}  /* namespace UI */
}  /* namespace Quoridor */
