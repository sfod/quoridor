#ifndef QUORIDOR_UI_MENU_HPP_
#define QUORIDOR_UI_MENU_HPP_

#include <string>
#include <vector>

namespace Quoridor {
namespace UI {

class Menu {
public:
    Menu() {}
    virtual ~Menu() {}

    virtual void up() = 0;
    virtual void down() = 0;
    virtual std::string item() = 0;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_MENU_HPP_ */
