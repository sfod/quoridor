#ifndef QUORIDOR_UI_HPP_
#define QUORIDOR_UI_HPP_

#include "ui_impl.hpp"

namespace Quoridor {
namespace UI {

class UI {
public:
    UI();
    virtual ~UI();

    virtual void draw_window();
    virtual void update(const std::vector<std::vector<char>> &repr);

private:
    UIImpl *ui_impl_;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_HPP_ */
