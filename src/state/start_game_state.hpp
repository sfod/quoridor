#ifndef QUORIDOR_START_GAME_STATE_HPP_
#define QUORIDOR_START_GAME_STATE_HPP_

#include <vector>

#include "player_factory.hpp"
#include "game.hpp"
#include "iplayer.hpp"
#include "istate.hpp"

#include "UI/window.hpp"
#include "UI/menu.hpp"

namespace Quoridor {

class StartGameState : public IState {
public:
    explicit StartGameState(std::shared_ptr<UI::UI> ui);
    virtual ~StartGameState();

    virtual void handle_events(StateManager *stm, std::shared_ptr<UI::UI> ui);
    virtual void update();
    virtual void draw(std::shared_ptr<UI::UI> ui);
    virtual void change_state();

    virtual void set_player(int i, std::shared_ptr<IPlayer> player);

private:
    std::shared_ptr<UI::Window> win_;
    std::shared_ptr<UI::Menu> menu_;
    PlayerFactory pf_;
    std::vector<std::shared_ptr<IPlayer>> players_;
    size_t player_num_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_START_GAME_STATE_HPP_ */
