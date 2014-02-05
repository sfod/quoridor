#ifndef QUORIDOR_UI_HPP_
#define QUORIDOR_UI_HPP_

#include <memory>
#include "UI/menu.hpp"
#include "UI/window.hpp"

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

class UI {
public:
    UI() {}
    virtual ~UI() {}

    virtual std::shared_ptr<Window> create_window() = 0;
    virtual std::shared_ptr<Menu> create_menu(const std::vector<std::string> &items) = 0;
    virtual bool poll_event(Event *ev) = 0;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_HPP_ */
