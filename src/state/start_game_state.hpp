#ifndef QUORIDOR_START_GAME_STATE_HPP_
#define QUORIDOR_START_GAME_STATE_HPP_

#include <vector>

#include "istate.hpp"
#include "UI/window.hpp"
#include "UI/menu.hpp"

namespace Quoridor {

class StartGameState : public IState {
public:
    explicit StartGameState(std::shared_ptr<UI::UI> ui);
    virtual ~StartGameState();

    virtual void handle_events(StateManager *stm);
    virtual void update();
    virtual void draw();
    virtual void change_state();

private:
    std::shared_ptr<UI::Window> win_;
    std::shared_ptr<UI::Menu> menu_;
    std::vector<std::string> player_types_;
    size_t player_num_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_START_GAME_STATE_HPP_ */
