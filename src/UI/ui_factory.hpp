#ifndef QUORIDOR_UI_FACTORY_HPP_
#define QUORIDOR_UI_FACTORY_HPP_

#include <memory>
#include "ui.hpp"

namespace Quoridor {
namespace UI {

class UIFactory {
public:
    UIFactory();
    ~UIFactory();

    virtual std::shared_ptr<UI> make_ui(const std::string &ui_type) const;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_FACTORY_HPP_ */
