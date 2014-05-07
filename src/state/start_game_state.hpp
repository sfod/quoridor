#ifndef QUORIDOR_START_GAME_STATE_HPP_
#define QUORIDOR_START_GAME_STATE_HPP_

#include <vector>

#include <CEGUI/CEGUI.h>

#include "istate.hpp"


namespace Quoridor {

class StartGameState : public IState {
public:
    explicit StartGameState(std::shared_ptr<StateManager> stm);
    virtual ~StartGameState();

    virtual std::shared_ptr<CEGUI::Window> window() const;

private:
    void subscribe_for_events_();

private:
    std::shared_ptr<StateManager> stm_;
    std::shared_ptr<CEGUI::Window> win_;
    std::vector<std::string> player_types_;
    size_t player_num_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_START_GAME_STATE_HPP_ */
