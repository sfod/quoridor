#ifndef QUORIDOR_NCURSES_UI_IMPL_HPP_
#define QUORIDOR_NCURSES_UI_IMPL_HPP_

#include "ui_impl.hpp"

namespace Quoridor {
namespace UI {

class NcursesUIImpl : public UIImpl {
public:
    NcursesUIImpl();
    ~NcursesUIImpl();

    virtual void draw_window();
    virtual void update(const std::vector<std::vector<char>> &repr);
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_NCURSES_UI_IMPL_HPP_ */
