#ifndef QUORIDOR_NCURSES_UI_IMPL_HPP_
#define QUORIDOR_NCURSES_UI_IMPL_HPP_

#include <memory>

#include <ncurses.h>

#include "ui_impl.hpp"
#include "ncurses/ncurses_menu.hpp"

namespace Quoridor {
namespace UI {

class NcursesUIImpl : public UIImpl {
public:
    NcursesUIImpl();
    ~NcursesUIImpl();

    virtual void draw_window();
    virtual void update(const std::vector<std::vector<char>> &repr);

    virtual void add_menu(const std::vector<std::string> &items);
    virtual void up_menu();
    virtual void down_menu();
    virtual std::string menu_item();

private:
    WINDOW *win_;
    std::shared_ptr<NcursesMenu> menu_;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_NCURSES_UI_IMPL_HPP_ */
