#include "ncurses_ui.hpp"
#include "ncurses_window.hpp"
#include "ncurses_menu.hpp"

namespace Quoridor {
namespace UI {

NcursesUI::NcursesUI()
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

NcursesUI::~NcursesUI()
{
    endwin();
}

std::shared_ptr<Window> NcursesUI::create_window()
{
    return std::shared_ptr<Window>(new NcursesWindow());
}

std::shared_ptr<Menu> NcursesUI::create_menu(const std::vector<std::string> &items)
{
    return std::shared_ptr<Menu>(new NcursesMenu(items));
}

bool NcursesUI::poll_event(Event *ev)
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
