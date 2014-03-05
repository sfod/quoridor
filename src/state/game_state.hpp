#ifndef QUORIDOR_GAME_STATE_HPP_
#define QUORIDOR_GAME_STATE_HPP_

#include <vector>

#include "istate.hpp"

#include "board.hpp"
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
    std::shared_ptr<Pawn> next_pawn() const;

private:
    std::shared_ptr<UI::Window> win_;
    std::shared_ptr<Board> board_;
    PlayerFactory pf_;
    std::map<std::shared_ptr<Pawn>, std::shared_ptr<IPlayer>> players_;
    std::vector<std::shared_ptr<Pawn>> pawn_list_;
    std::shared_ptr<Pawn> cur_pawn_;
    mutable std::vector<std::vector<char>> repr_;
    bool is_running_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_GAME_STATE_HPP_ */
