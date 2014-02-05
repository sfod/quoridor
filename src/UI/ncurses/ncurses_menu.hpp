#ifndef QUORIDOR_NCURSES_UI_MENU_HPP_
#define QUORIDOR_NCURSES_UI_MENU_HPP_

#include <string>
#include <vector>

#include <menu.h>

#include "UI/menu.hpp"

namespace Quoridor {
namespace UI {

class NcursesMenu : public Menu {
public:
    explicit NcursesMenu(const std::vector<std::string> &items);
    ~NcursesMenu();

    virtual void up();
    virtual void down();
    virtual std::string item();

private:
    MENU *menu_;
    ITEM **items_;
    size_t item_num_;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_NCURSES_UI_MENU_HPP_ */
