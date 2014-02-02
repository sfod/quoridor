#ifndef QUORIDOR_GAME_STATE_HPP_
#define QUORIDOR_GAME_STATE_HPP_

#include "istate.hpp"

namespace Quoridor {

class GameState : public IState {
public:
    GameState();
    virtual ~GameState();

    virtual void handle_events(StateManager *stm, std::shared_ptr<UI::UIImpl> ui);
    virtual void update();
    virtual void draw();
    virtual void change_state();
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_GAME_STATE_HPP_ */
