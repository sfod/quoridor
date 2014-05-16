#ifndef QUORIDOR_START_GAME_STATE_HPP_
#define QUORIDOR_START_GAME_STATE_HPP_

#include <vector>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/identity.hpp>

#include <CEGUI/CEGUI.h>

#include "istate.hpp"
#include "state_manager.hpp"


struct plist_handler_t {
    CEGUI::String name;
    std::function<void()> handler;
};

typedef boost::multi_index_container<
    plist_handler_t,
    boost::multi_index::indexed_by<
        boost::multi_index::random_access<>,
        boost::multi_index::ordered_unique<
            boost::multi_index::member<plist_handler_t, CEGUI::String, &plist_handler_t::name>
        >
    >
> plist_handler_list_t;

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
    plist_handler_list_t plist_handlers_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_START_GAME_STATE_HPP_ */
