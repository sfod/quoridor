#ifndef QUORIDOR_UI_FACTORY_HPP_
#define QUORIDOR_UI_FACTORY_HPP_

#include <memory>
#include "ui_impl.hpp"

namespace Quoridor {
namespace UI {

class UIFactory {
public:
    UIFactory();
    ~UIFactory();

    virtual std::shared_ptr<UIImpl> make_ui_impl() const;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_FACTORY_HPP_ */
