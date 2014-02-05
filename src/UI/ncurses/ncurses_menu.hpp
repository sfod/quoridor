#ifndef QUORIDOR_NCURSES_UI_MENU_HPP_
#define QUORIDOR_NCURSES_UI_MENU_HPP_

#include <string>
#include <vector>
#include <menu.h>

namespace Quoridor {
namespace UI {

class NcursesMenu {
public:
    NcursesMenu(WINDOW *win, const std::vector<std::string> &items);
    ~NcursesMenu();

    void up();
    void down();
    std::string item();

private:
    MENU *menu_;
    ITEM **items_;
    size_t item_num_;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_NCURSES_UI_MENU_HPP_ */
