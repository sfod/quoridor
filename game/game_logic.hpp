#pragma once

#include <memory>
#include <vector>

#include <QObject>

#include "events/event_manager.hpp"

#include "actors/actor_factory.hpp"
#include "actors/actor_keeper.hpp"
#include "graph/graph.hpp"
#include "view/iview.hpp"
#include "player_handler.hpp"

enum class LogicState {
    LS_Uninitialized,
    LS_MainMenu,
    LS_Options,
    LS_Game,
    LS_Invalid
};

class GameLogic {

    typedef std::pair<std::shared_ptr<Actor>, PlayerType> player_actor_t;

public:
    explicit GameLogic(QObject *qroot);
    ~GameLogic();

    std::shared_ptr<ActorKeeper> actor_keeper() const { return actor_keeper_; }
    std::shared_ptr<Graph> graph() const { return graph_; }

    void change_state(LogicState state);
    void add_view(std::shared_ptr<IView> view);
    void change_view(std::shared_ptr<IView> view);

    void main_menu_win_delegate(const std::shared_ptr<EventData> &event);
    void options_win_delegate(const std::shared_ptr<EventData> &event);
    void game_win_delegate(const std::shared_ptr<EventData> &event);
    void game_terminated_delegate(const std::shared_ptr<EventData> &event);
    void req_actor_new_delegate(const std::shared_ptr<EventData> &event);
    void req_actor_move_delegate(const std::shared_ptr<EventData> &event);
    void req_set_wall(const std::shared_ptr<EventData> &even);

private:
    void register_delegates();
    void create_player(int idx, PlayerType ptype);
    void set_players();

private:
    LogicState state_;
    QObject *qroot_;
    std::list<bs2::connection> conn_list_;
    std::shared_ptr<ActorFactory> actor_factory_;
    std::shared_ptr<ActorKeeper> actor_keeper_;
    std::list<player_actor_t> player_list_;
    std::shared_ptr<Graph> graph_;
    std::vector<std::shared_ptr<IView>> view_list_;
    int player_idx_;
    PlayerHandler player_handler_;
};
