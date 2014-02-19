#ifndef QUORIDOR_GAME_STATE_HPP_
#define QUORIDOR_GAME_STATE_HPP_

#include <vector>

#include "istate.hpp"
#include "game.hpp"
#include "player_factory.hpp"
#include "iplayer.hpp"
#include "UI/window.hpp"

namespace Quoridor {

class GameState : public IState {
public:
    GameState(std::shared_ptr<UI::UI> ui,
            const std::vector<std::string> &player_types);
    virtual ~GameState();

    virtual void handle_events(StateManager *stm);
    virtual void update();
    virtual void draw();
    virtual void change_state();

private:
    void init_board_repr() const;
    void draw_wall(const Wall &wall) const;
    void redraw_pawn(char p, const pos_t &old_pos, const pos_t &new_pos) const;

private:
    std::shared_ptr<UI::Window> win_;
    std::shared_ptr<Game> game_;
    PlayerFactory pf_;
    std::vector<std::shared_ptr<IPlayer>> players_;
    mutable std::vector<std::vector<char>> repr_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_GAME_STATE_HPP_ */
