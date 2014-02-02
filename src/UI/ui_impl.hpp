#ifndef QUORIDOR_UI_IMPL_HPP_
#define QUORIDOR_UI_IMPL_HPP_

#include <vector>

namespace Quoridor {
namespace UI {

enum Event {
    kEnter,
    kUp,
    kRight,
    kDown,
    kLeft,
    kEsc,
    kNone
};

class UIImpl {
public:
    virtual ~UIImpl() {}

    virtual void draw_window() = 0;
    virtual void update(const std::vector<std::vector<char>> &repr) = 0;
    virtual bool poll_event(Event *ev) = 0;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_IMPL_HPP_ */
