#ifndef QUORIDOR_GAME_STATE_HPP_
#define QUORIDOR_GAME_STATE_HPP_

#include <vector>

#include "game.hpp"
#include "iplayer.hpp"
#include "istate.hpp"

namespace Quoridor {

class GameState : public IState {
public:
    GameState();
    virtual ~GameState();

    virtual void handle_events(StateManager *stm, std::shared_ptr<UI::UI> ui);
    virtual void update();
    virtual void draw(std::shared_ptr<UI::UI> ui);
    virtual void change_state();

private:
    void init_board_repr() const;

private:
    std::shared_ptr<Game> game_;
    std::vector<IPlayer> players_;
    mutable std::vector<std::vector<char>> repr_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_GAME_STATE_HPP_ */
