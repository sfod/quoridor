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
    enum GameStatus {
        kWaitingForMove,
        kPerformedMove,
        kNeedPawnRedraw,
        kWaitingForAnimationEnd,
        kFinished
    };

private:
    void set_pawns_();
    void redraw_pawn_();
    std::shared_ptr<Pawn> next_pawn() const;
    void make_move();
    bool is_finished() const;

private:
    void subscribe_for_events_();
    bool handle_back_(const CEGUI::EventArgs &e);
    bool handle_end_anim_(const CEGUI::EventArgs &e);

private:
    static std::string name_;

private:
    std::shared_ptr<StateManager> stm_;
    std::shared_ptr<CEGUI::Window> win_;
    std::shared_ptr<CEGUI::Animation> anim_;
    std::shared_ptr<Board> board_;
    PlayerFactory pf_;
    std::map<std::shared_ptr<Pawn>, std::shared_ptr<IPlayer>> players_;
    std::vector<std::shared_ptr<Pawn>> pawn_list_;
    std::shared_ptr<Pawn> cur_pawn_;
    std::vector<Pos> pawn_path_;
    GameStatus status_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_GAME_STATE_HPP_ */
