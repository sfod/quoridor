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

bool NcursesUIImpl::poll_event(Event *ev)
{
    int ch;
    ch = getch();
    switch (ch) {
    case KEY_ENTER:
    case '\n':
    case '\r':
        *ev = kEnter;
        break;
    case KEY_UP:
        *ev = kUp;
        break;
    case KEY_RIGHT:
        *ev = kRight;
        break;
    case KEY_DOWN:
        *ev = kDown;
        break;
    case KEY_LEFT:
        *ev = kLeft;
        break;
    case 'q':
        *ev = kEsc;
        break;
    default:
        *ev = kNone;
        break;
    }
    return true;
}

}  /* namespace UI */
}  /* namespace Quoridor */
