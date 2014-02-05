#ifndef QUORIDOR_UI_WINDOW_HPP_
#define QUORIDOR_UI_WINDOW_HPP_

#include <vector>

namespace Quoridor {
namespace UI {

class Window {
public:
    Window() {}
    virtual ~Window() {}

    virtual void draw_board(const std::vector<std::vector<char>> &repr) = 0;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_WINDOW_HPP_ */
