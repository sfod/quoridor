#ifndef QUORIDOR_MAIN_MENU_STATE_HPP_
#define QUORIDOR_MAIN_MENU_STATE_HPP_

#include "istate.hpp"

namespace Quoridor {

class MainMenuState : public IState {
public:
    explicit MainMenuState(std::shared_ptr<UI::UI> ui);
    virtual ~MainMenuState();

    virtual void handle_events(StateManager *stm);
    virtual void update();
    virtual void draw();
    virtual void change_state();

private:
    std::shared_ptr<UI::Window> win_;
    std::shared_ptr<UI::Menu> menu_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_MAIN_MENU_STATE_HPP_ */
