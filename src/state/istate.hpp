#ifndef QUORIDOR_ISTATE_HPP_
#define QUORIDOR_ISTATE_HPP_

#include <memory>
#include <CEGUI/CEGUI.h>
#include "state_manager.hpp"

namespace Quoridor {

class StateManager;

class IState {
public:
    IState() {}
    virtual ~IState() {}

    virtual void handle_events(StateManager *stm) = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void change_state() = 0;
    virtual std::shared_ptr<CEGUI::Window> window() const = 0;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_ISTATE_HPP_ */
