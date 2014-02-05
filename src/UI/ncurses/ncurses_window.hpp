#ifndef QUORIDOR_NCURSES_WINDOW_HPP_
#define QUORIDOR_NCURSES_WINDOW_HPP_

#include <memory>
#include <string>
#include <vector>

#include <ncurses.h>

#include "UI/window.hpp"

namespace Quoridor {
namespace UI {

class NcursesWindow : public Window {
public:
    NcursesWindow();
    ~NcursesWindow();

    virtual void draw_board(const std::vector<std::vector<char>> &repr);

private:
    WINDOW *win_;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_NCURSES_WINDOW_HPP_ */
