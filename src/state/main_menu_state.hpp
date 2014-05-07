#ifndef QUORIDOR_MAIN_MENU_STATE_HPP_
#define QUORIDOR_MAIN_MENU_STATE_HPP_

#include <CEGUI/CEGUI.h>

#include "istate.hpp"

namespace Quoridor {

class MainMenuState : public IState {
public:
    explicit MainMenuState();
    virtual ~MainMenuState();

    virtual void handle_events(StateManager *stm);
    virtual void update();
    virtual void draw();
    virtual void change_state();
    virtual std::shared_ptr<CEGUI::Window> window() const;

private:
    void subscribe_for_events_();
    bool handle_start_game_(const CEGUI::EventArgs &e);

private:
    std::shared_ptr<CEGUI::Window> win_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_MAIN_MENU_STATE_HPP_ */
