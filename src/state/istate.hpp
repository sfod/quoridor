#ifndef QUORIDOR_ISTATE_HPP_
#define QUORIDOR_ISTATE_HPP_

#include <memory>
#include <string>
#include <CEGUI/CEGUI.h>

namespace Quoridor {

class IState {
public:
    IState() {}
    virtual ~IState() {}

    virtual std::shared_ptr<CEGUI::Window> window() const = 0;
    virtual const std::string &name() const = 0;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_ISTATE_HPP_ */
