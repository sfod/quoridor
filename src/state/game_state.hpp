#ifndef QUORIDOR_GAME_STATE_HPP_
#define QUORIDOR_GAME_STATE_HPP_

#include <memory>
#include <vector>

#include "istate.hpp"
#include "game.hpp"
#include "player/iplayer.hpp"
#include "player/player_factory.hpp"
#include "pos_utils.hpp"
#include "state_manager.hpp"

#include "cegui_ext/draggable_window.hpp"

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
        kPreparingMove,
        kNeedMove,
        kWaitingForMove,
        kPerformedMove,
        kNeedPawnRedraw,
        kNeedDrawWall,
        kWaitingForAnimationEnd,
        kFinished
    };

private:
    void init_gui_();
    void set_pawns_();
    void init_walls_();
    void redraw_pawn_();
    void draw_wall_();
    void pre_process_move_();
    void post_process_move_();
    void switch_cur_pawn_();
    void make_move_();
    int move_pawn_(const Node &node);
    int add_wall_(const Wall &wall);
    bool is_finished_() const;
    void callback_move(move_t move);

private:
    void subscribe_for_events_();
    bool handle_back_(const CEGUI::EventArgs &e);
    bool handle_end_anim_(const CEGUI::EventArgs &e);
    bool handle_window_dropped_(const CEGUI::EventArgs &e);
    bool handle_pawn_dropped_(const CEGUI_Ext::DragEvent &de);
    bool handle_wall_dropped_(const CEGUI_Ext::DragEvent &de);
    Node normalize_pawn_pos_(const CEGUI::Vector2f &rel_pos);
    Wall normalize_wall_pos_(const CEGUI::Vector2f &rel_pos);

private:
    static std::string name_;

private:
    std::shared_ptr<StateManager> stm_;
    std::shared_ptr<CEGUI::Window> win_;
    std::shared_ptr<CEGUI::Animation> anim_;
    std::shared_ptr<Game> game_;
    PlayerFactory pf_;
    std::map<std::shared_ptr<Pawn>, std::shared_ptr<IPlayer>> players_;
    std::vector<std::shared_ptr<Pawn>> pawn_list_;
    std::shared_ptr<Pawn> cur_pawn_;
    std::map<std::shared_ptr<Pawn>, CEGUI_Ext::DraggableWindow*> drag_list_;
    std::map<CEGUI::Window*, std::shared_ptr<Pawn>> pawn_wins_;
    std::map<CEGUI::Window*, int> wall_wins_;
    std::vector<Node> pawn_path_;
    Wall added_wall_;
    size_t wall_idx_;
    GameStatus status_;
    PosUtils pos_utils_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_GAME_STATE_HPP_ */
