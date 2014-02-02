#ifndef QUORIDOR_MAIN_MENU_STATE_HPP_
#define QUORIDOR_MAIN_MENU_STATE_HPP_

#include "istate.hpp"

namespace Quoridor {

class MainMenuState : public IState {
public:
    MainMenuState();
    virtual ~MainMenuState();

    virtual void handle_events(StateManager *stm, std::shared_ptr<UI::UIImpl> ui);
    virtual void update();
    virtual void draw();
    virtual void change_state();
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_MAIN_MENU_STATE_HPP_ */
