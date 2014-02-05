#ifndef QUORIDOR_NCURSES_UI_HPP_
#define QUORIDOR_NCURSES_UI_HPP_

#include "UI/ui.hpp"

namespace Quoridor {
namespace UI {

class NcursesUI : public UI {
public:
    NcursesUI();
    virtual ~NcursesUI();

    virtual std::shared_ptr<Window> create_window();
    virtual std::shared_ptr<Menu> create_menu(const std::vector<std::string> &items);
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_NCURSES_UI_HPP_ */
