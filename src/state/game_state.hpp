#ifndef QUORIDOR_GAME_STATE_HPP_
#define QUORIDOR_GAME_STATE_HPP_

#include <memory>
#include <vector>

#include "board.hpp"
#include "iplayer.hpp"
#include "istate.hpp"
#include "player_factory.hpp"
#include "state_manager.hpp"

namespace Quoridor {

class GameState : public IState {
public:
    GameState(std::shared_ptr<StateManager> stm,
            const std::vector<std::string> &player_types);
    virtual ~GameState();
    virtual void update();
    virtual std::shared_ptr<CEGUI::Window> window() const;
    virtual const std::string &name() const;

private:
    void subscribe_for_events_();
    bool handle_fields_(const CEGUI::EventArgs &e);

private:
    void init_board_repr() const;
    void draw_wall(const Wall &wall) const;
    void redraw_pawn(char p, const Pos &old_pos, const Pos &new_pos) const;
    std::shared_ptr<Pawn> next_pawn() const;

private:
    static std::string name_;

private:
    std::shared_ptr<StateManager> stm_;
    std::shared_ptr<CEGUI::Window> win_;
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
