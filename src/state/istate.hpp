#ifndef QUORIDOR_ISTATE_HPP_
#define QUORIDOR_ISTATE_HPP_

#include <memory>
#include "state_manager.hpp"
#include "UI/ui.hpp"

namespace Quoridor {

class StateManager;

class IState {
public:
    IState() {}
    virtual ~IState() {}

    virtual void handle_events(StateManager *stm, std::shared_ptr<UI::UI> ui) = 0;
    virtual void update() = 0;
    virtual void draw(std::shared_ptr<UI::UI> ui) = 0;
    virtual void change_state() = 0;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_ISTATE_HPP_ */
