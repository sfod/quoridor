#ifndef QUORIDOR_UI_IMPL_HPP_
#define QUORIDOR_UI_IMPL_HPP_

#include <string>
#include <vector>

namespace Quoridor {
namespace UI {

class UIImpl {
public:
    virtual ~UIImpl() {}

    virtual void draw_window() = 0;
    virtual void update(const std::vector<std::vector<char>> &repr) = 0;

    virtual void add_menu(const std::vector<std::string> &items) = 0;
    virtual void up_menu() = 0;
    virtual void down_menu() = 0;
    virtual std::string menu_item() = 0;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_IMPL_HPP_ */
