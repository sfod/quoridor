#ifndef QUORIDOR_START_GAME_STATE_HPP_
#define QUORIDOR_START_GAME_STATE_HPP_

#include <vector>

#include <CEGUI/CEGUI.h>

#include "istate.hpp"
#include "state_manager.hpp"


namespace Quoridor {

class StartGameState : public IState {
public:
    explicit StartGameState(std::shared_ptr<StateManager> stm);
    virtual ~StartGameState();
    virtual void update();
    virtual std::shared_ptr<CEGUI::Window> window() const;
    virtual const std::string &name() const;

private:
    void init_win_();
    void init_player_list_(std::shared_ptr<CEGUI::DefaultWindow> win, size_t n);
    void set_player_num_();

private:
    void subscribe_for_events_();
    bool handle_start_game_(const CEGUI::EventArgs &e);
    bool handle_return_(const CEGUI::EventArgs &e);
    bool handle_player_num_(const CEGUI::EventArgs &e);
    void set_player_list2_();
    void set_player_list4_();

private:
    static std::string name_;
    std::shared_ptr<StateManager> stm_;
    std::shared_ptr<CEGUI::Window> win_;
    std::shared_ptr<CEGUI::DefaultWindow> plist2_win_;
    std::shared_ptr<CEGUI::DefaultWindow> plist4_win_;
    std::shared_ptr<CEGUI::DefaultWindow> cur_plist_win_;
    std::map<CEGUI::String, std::function<void()>> plist_handlers_;
    std::vector<std::string> player_types_;
    size_t player_num_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_START_GAME_STATE_HPP_ */
