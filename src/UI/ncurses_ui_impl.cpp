#include "ncurses_ui_impl.hpp"
#include <ncurses.h>

namespace Quoridor {
namespace UI {

NcursesUIImpl::NcursesUIImpl()
{
}

NcursesUIImpl::~NcursesUIImpl()
{
    endwin();
}

void NcursesUIImpl::draw_window()
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

void NcursesUIImpl::update(const std::vector<std::vector<char>> &repr)
{
    move(0, 0);
    for (auto cols : repr) {
        for (auto col : cols) {
            addch(col);
        }
        addch('\n');
    }
    addch('\n');
    refresh();
}

}  /* namespace UI */
}  /* namespace Quoridor */
