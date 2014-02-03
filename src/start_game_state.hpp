#ifndef QUORIDOR_START_GAME_STATE_HPP_
#define QUORIDOR_START_GAME_STATE_HPP_

#include <vector>

#include "game.hpp"
#include "iplayer.hpp"
#include "istate.hpp"

namespace Quoridor {

class StartGameState : public IState {
public:
    StartGameState();
    virtual ~StartGameState();

    virtual void handle_events(StateManager *stm, std::shared_ptr<UI::UIImpl> ui);
    virtual void update();
    virtual void draw(std::shared_ptr<UI::UIImpl> ui);
    virtual void change_state();

    virtual void set_player(int i, std::shared_ptr<IPlayer> player);

private:
    std::vector<std::shared_ptr<IPlayer>> players_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_START_GAME_STATE_HPP_ */
