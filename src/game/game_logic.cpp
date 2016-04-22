#include "game_logic.hpp"
#include <QDebug>

#include "view/main_menu_view.hpp"
#include "view/options_view.hpp"
#include "view/game_view.hpp"
#include "view/ai_view.hpp"
#include "actors/graph_component.hpp"
#include "actors/wall_component.hpp"

#include "events/event_data_main_menu.hpp"
#include "events/event_data_options.hpp"
#include "events/event_data_game.hpp"
#include "events/event_data_new_actor.hpp"
#include "events/event_data_request_new_actor.hpp"
#include "events/event_data_request_actor_move.hpp"
#include "events/event_data_request_set_wall.hpp"
#include "events/event_data_move_actor.hpp"
#include "events/event_data_set_wall.hpp"
#include "events/event_data_set_actor_possible_moves.hpp"
#include "events/event_data_set_actor_active.hpp"
#include "events/event_data_game_finished.hpp"
#include "events/event_data_game_terminated.hpp"

GameLogic::GameLogic(QObject *qroot)
    : state_(LogicState::LS_Uninitialized), qroot_(qroot),
    actor_factory_(new ActorFactory), actor_keeper_(new ActorKeeper),
    player_list_(), graph_(new Graph), view_list_(), player_idx_(1),
    player_handler_()
{
    register_delegates();
}

void GameLogic::change_state(LogicState state)
{
    std::shared_ptr<IView> view;

    switch (state) {
    case LogicState::LS_Uninitialized:
        break;
    case LogicState::LS_MainMenu:
        view.reset(new MainMenuView(qroot_));
        change_view(view);
        break;
    case LogicState::LS_Options:
        view.reset(new OptionsView(qroot_));
        change_view(view);
        break;
    case LogicState::LS_Game: {
        view_list_.clear();
        bool is_main = true;
        for (auto actor : player_list_) {
            switch(actor.second) {
            case PlayerType::PT_Human:
                view.reset(new GameView(qroot_, is_main));
                is_main = false;
                break;
            case PlayerType::PT_AI:
                view.reset(new AIView(actor_keeper_));
                break;
            case PlayerType::PT_Invalid:
            default:
                qDebug() << "invalid player type";
                break;
            }

            add_view(view);
            view->attach(actor.first->id());
        }
        set_players();
        break;
    }
    case LogicState::LS_Invalid:
        break;
    default:
        break;
    }
}

void GameLogic::add_view(std::shared_ptr<IView> view)
{
    view_list_.push_back(view);
}

void GameLogic::change_view(std::shared_ptr<IView> view)
{
    if (!view_list_.empty()) {
        view_list_.pop_back();
    }
    view_list_.push_back(view);
}

void GameLogic::main_menu_win_delegate(const std::shared_ptr<EventData> &/*event*/)
{
    change_state(LogicState::LS_MainMenu);
}

void GameLogic::options_win_delegate(const std::shared_ptr<EventData> &/*event*/)
{
    change_state(LogicState::LS_Options);
}

void GameLogic::game_win_delegate(const std::shared_ptr<EventData> &/*event*/)
{
    change_state(LogicState::LS_Game);
}

void GameLogic::game_terminated_delegate(const std::shared_ptr<EventData> &/*event*/)
{
    player_list_.clear();
    player_idx_ = 1;
    player_handler_.clear();
}

void GameLogic::req_actor_new_delegate(const std::shared_ptr<EventData> &event)
{
    auto req_new_event = std::dynamic_pointer_cast<EventData_RequestNewActor>(event);
    create_player(player_idx_, req_new_event->player_type());
    ++player_idx_;
}

void GameLogic::req_actor_move_delegate(const std::shared_ptr<EventData> &event)
{
    auto req_move_event = std::dynamic_pointer_cast<EventData_RequestActorMove>(event);

    const std::shared_ptr<Actor> &actor = actor_keeper_->actor(req_move_event->actor_id());
    if (actor) {
        auto graph_comp = std::dynamic_pointer_cast<GraphComponent>(
                    actor->component(GraphComponent::id())
        );

        // TODO(?) move this logic into GraphComponent
        if (graph_comp->move_actor(req_move_event->node())) {
            // update active player position
            auto move_event = std::make_shared<EventData_MoveActor>(
                    actor->id(),
                    graph_comp->node(),
                    graph_comp->possible_moves());
            EventManager::get()->queue_event(move_event);

            // update other players possible moves
            for (auto player_actor : player_list_) {
                ActorId aid = player_actor.first->id();
                // active player possible moves are already updated
                if (aid == actor->id()) {
                    continue;
                }

                auto gcomp = std::dynamic_pointer_cast<GraphComponent>(
                            player_actor.first->component(GraphComponent::id())
                );
                auto pos_move_event = std::make_shared<EventData_SetActorPossibleMoves>(
                            aid, gcomp->possible_moves()
                );
                EventManager::get()->queue_event(pos_move_event);
            }

            if (graph_comp->is_at_goal_node()) {
                auto game_finished_event = std::make_shared<EventData_GameFinished>(actor->id());
                EventManager::get()->queue_event(game_finished_event);
            }
            else {
                auto active_player = player_handler_.next_player();
                auto act_event = std::make_shared<EventData_SetActorActive>(active_player);
                EventManager::get()->queue_event(act_event);
            }
        }
    }
}

void GameLogic::req_set_wall(const std::shared_ptr<EventData> &event)
{
    auto req_wall_event = std::dynamic_pointer_cast<EventData_RequestSetWall>(event);
    const std::shared_ptr<Actor> &actor = actor_keeper_->actor(req_wall_event->actor_id());
    if (actor) {
        auto graph_comp = std::dynamic_pointer_cast<GraphComponent>(
                    actor->component(GraphComponent::id())
        );

        const Wall &wall = req_wall_event->wall();
        if (graph_comp && graph_comp->set_wall(wall)) {
            // update active player position
            auto set_wall_event = std::make_shared<EventData_SetWall>(actor->id(), wall);
            EventManager::get()->queue_event(set_wall_event);

            // update other players possible moves
            for (auto player_actor : player_list_) {
                ActorId aid = player_actor.first->id();
                // active player possible moves will be updated on the next player turn
                if (aid == actor->id()) {
                    continue;
                }

                auto gcomp = std::dynamic_pointer_cast<GraphComponent>(
                            player_actor.first->component(GraphComponent::id())
                );
                auto pos_move_event = std::make_shared<EventData_SetActorPossibleMoves>(
                            aid, gcomp->possible_moves()
                );
                EventManager::get()->queue_event(pos_move_event);
            }

            auto active_player = player_handler_.next_player();
            auto act_event = std::make_shared<EventData_SetActorActive>(active_player);
            EventManager::get()->queue_event(act_event);
        }
        else {
            qDebug() << "failed to set wall";
        }
    }
}

void GameLogic::register_delegates()
{
    EventManager::get()->add_listener(this,
            std::bind(&GameLogic::main_menu_win_delegate, this, std::placeholders::_1),
            EventData_MainMenu::static_event_type());

    EventManager::get()->add_listener(this,
            std::bind(&GameLogic::options_win_delegate, this, std::placeholders::_1),
            EventData_Options::static_event_type());

    EventManager::get()->add_listener(this,
            std::bind(&GameLogic::game_win_delegate, this, std::placeholders::_1),
            EventData_Game::static_event_type());

    EventManager::get()->add_listener(this,
            std::bind(&GameLogic::game_terminated_delegate, this, std::placeholders::_1),
            EventData_GameTerminated::static_event_type());

    EventManager::get()->add_listener(this,
            std::bind(&GameLogic::req_actor_new_delegate, this, std::placeholders::_1),
            EventData_RequestNewActor::static_event_type());

    EventManager::get()->add_listener(this,
            std::bind(&GameLogic::req_actor_move_delegate, this, std::placeholders::_1),
            EventData_RequestActorMove::static_event_type());

    EventManager::get()->add_listener(this,
            std::bind(&GameLogic::req_set_wall, this, std::placeholders::_1),
            EventData_RequestSetWall::static_event_type());
}

void GameLogic::create_player(int idx, PlayerType ptype)
{
    QString player_cfg_file(":/configs/player_" + player_type_to_str.at(ptype) + ".json");
    std::vector<QString> component_resources = {
        QString(":/configs/player_position_" + QString::number(idx) + ".json"),
        QString(":/configs/player_wall_number_" + QString::number(2) + ".json")  // FIXME use number of players
    };
    std::shared_ptr<Actor> actor = actor_factory_->create_actor(graph_, player_cfg_file, component_resources);
    actor_keeper_->add_actor(actor);
    player_list_.push_back(player_actor_t(actor, ptype));
    player_handler_.add_player(actor->id());
}

void GameLogic::set_players()
{
    for (auto actor : player_list_) {
        auto graph_comp = std::dynamic_pointer_cast<GraphComponent>(
                    actor.first->component(GraphComponent::id())
        );
        auto wall_comp = std::dynamic_pointer_cast<WallComponent>(
                    actor.first->component(WallComponent::id())
        );
        auto new_event = std::make_shared<EventData_NewActor>(
                    actor.first->id(),
                    graph_comp->node(),
                    graph_comp->possible_moves(),
                    wall_comp->wall_number()
        );
        EventManager::get()->trigger_event(new_event);
    }

    ActorId active_player = player_handler_.next_player();
    auto act_event = std::make_shared<EventData_SetActorActive>(active_player);
    EventManager::get()->queue_event(act_event);
}
