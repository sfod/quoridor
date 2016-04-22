#include "game_view.hpp"
#include <QQuickItem>
#include "events/event_data_options.hpp"
#include "events/event_data_new_actor.hpp"
#include "events/event_data_move_actor.hpp"
#include "events/event_data_set_wall.hpp"
#include "events/event_data_set_actor_possible_moves.hpp"
#include "events/event_data_set_actor_active.hpp"
#include "events/event_data_game_finished.hpp"
#include "events/event_data_new_actor.hpp"
#include "events/event_data_request_actor_move.hpp"
#include "events/event_data_request_set_wall.hpp"
#include "events/event_data_game_terminated.hpp"
#include "exceptions/exception.hpp"

GameView::GameView(QObject *qroot, std::shared_ptr<EventManager> event_manager, bool is_main)
    : QtView(NULL), qroot_(qroot), qboard_(), qrecorder_(),
      qbutton_(), actor_id_(-1), event_manager_(event_manager), is_main_(is_main)
{
    connect_objects();

    if (is_main_) {
        connect_button("buttonBackToOptions", SLOT(button_back_clicked()), &qbutton_);
    }

    event_manager_->add_listener(this,
            std::bind(&GameView::new_actor_delegate, this, std::placeholders::_1),
            EventData_NewActor::static_event_type());

    event_manager_->add_listener(this,
            std::bind(&GameView::move_actor_delegate, this, std::placeholders::_1),
            EventData_MoveActor::static_event_type());

    event_manager_->add_listener(this,
            std::bind(&GameView::set_wall_delegate, this, std::placeholders::_1),
            EventData_SetWall::static_event_type());

    event_manager_->add_listener(this,
            std::bind(&GameView::set_actor_possible_moves_delegate, this, std::placeholders::_1),
            EventData_SetActorPossibleMoves::static_event_type());

    event_manager_->add_listener(this,
            std::bind(&GameView::set_active_delegate, this, std::placeholders::_1),
            EventData_SetActorActive::static_event_type());

    event_manager_->add_listener(this,
            std::bind(&GameView::game_finished_delegate, this, std::placeholders::_1),
            EventData_GameFinished::static_event_type());

    if (is_main_) {
        QMetaObject::invokeMethod(qboard_, "init");
        QMetaObject::invokeMethod(qrecorder_, "init");
    }
}

void GameView::on_msg()
{
}

void GameView::on_update()
{
}

void GameView::attach(ActorId actor_id)
{
    actor_id_ = actor_id;
}

void GameView::new_actor_delegate(const std::shared_ptr<EventData> &event)
{
    auto new_actor_event = std::dynamic_pointer_cast<EventData_NewActor>(event);
    if (is_main_) {
        const Node &node = new_actor_event->node();
        int idx = (8 - node.row()) * 9 + node.col();

        QVariantList possible_idx_list;
        for (auto &node : new_actor_event->possible_moves()) {
            int idx = (8 - node.row()) * 9 + node.col();
            possible_idx_list << idx;
            qDebug() << "\tmove:" << node;
        }

        QMetaObject::invokeMethod(qboard_, "addPlayer",
                Q_ARG(QVariant, static_cast<int>(new_actor_event->actor_id())),
                Q_ARG(QVariant, idx),
                Q_ARG(QVariant, QVariant::fromValue(possible_idx_list)),
                Q_ARG(QVariant, new_actor_event->wall_num())
        );
    }
}

void GameView::move_actor_delegate(const std::shared_ptr<EventData> &event)
{
    auto move_event = std::dynamic_pointer_cast<EventData_MoveActor>(event);
    if (is_main_) {
        const Node &node = move_event->node();
        int idx = (8 - node.row()) * 9 + node.col();

        QVariantList possible_idx_list;
        for (auto &node : move_event->possible_moves()) {
            int idx = (8 - node.row()) * 9 + node.col();
            possible_idx_list << idx;
            qDebug() << "\tmove:" << node;
        }

        QMetaObject::invokeMethod(qboard_, "setPawnPos",
                Q_ARG(QVariant, static_cast<int>(move_event->actor_id())),
                Q_ARG(QVariant, idx),
                Q_ARG(QVariant, QVariant::fromValue(possible_idx_list)));

        QString msg = QString::number(static_cast<int>(move_event->actor_id()))
                + " moves to " + node.to_string();
        QMetaObject::invokeMethod(qrecorder_, "addMessage", Q_ARG(QVariant, msg));
    }
}

void GameView::set_actor_possible_moves_delegate(const std::shared_ptr<EventData> &event)
{
    auto pos_move_event = std::dynamic_pointer_cast<EventData_SetActorPossibleMoves>(event);
    if (is_main_) {
        QVariantList possible_idx_list;
        for (auto &node : pos_move_event->possible_moves()) {
            int idx = (8 - node.row()) * 9 + node.col();
            possible_idx_list << idx;
        }

        QMetaObject::invokeMethod(qboard_, "setPawnPossibleMoves",
                Q_ARG(QVariant, static_cast<int>(pos_move_event->actor_id())),
                Q_ARG(QVariant, QVariant::fromValue(possible_idx_list)));
    }
}

void GameView::set_wall_delegate(const std::shared_ptr<EventData> &event)
{
    auto set_wall_event = std::dynamic_pointer_cast<EventData_SetWall>(event);
    const Wall &wall = set_wall_event->wall();
    if (is_main_) {
        const Node &node = wall.node();
        QMetaObject::invokeMethod(qboard_, "setWall",
                Q_ARG(QVariant, static_cast<int>(set_wall_event->actor_id())),
                Q_ARG(QVariant, static_cast<int>(wall.orientation())),
                Q_ARG(QVariant, node.row()),
                Q_ARG(QVariant, node.col()));
        QString msg = QString::number(static_cast<int>(set_wall_event->actor_id()))
                + " set " + wall.to_string();
        QMetaObject::invokeMethod(qrecorder_, "addMessage", Q_ARG(QVariant, msg));
    }
}

void GameView::set_active_delegate(const std::shared_ptr<EventData> &event)
{
    auto active_event = std::dynamic_pointer_cast<EventData_SetActorActive>(event);
    if (active_event->actor_id() == actor_id_) {
        QMetaObject::invokeMethod(qboard_, "setActivePlayer",
                Q_ARG(QVariant, static_cast<int>(active_event->actor_id())));
    }
}

void GameView::game_finished_delegate(const std::shared_ptr<EventData> &event)
{
    if (is_main_) {
        auto game_finished_event = std::dynamic_pointer_cast<EventData_GameFinished>(event);
        QMetaObject::invokeMethod(qboard_, "finishGame");

        QString msg = QString::number(game_finished_event->actor_id()) + " wins";
        QMetaObject::invokeMethod(qrecorder_, "addMessage", Q_ARG(QVariant, msg));
    }
}

void GameView::on_pawn_dropped(int actor_id, int idx)
{
    if (static_cast<ActorId>(actor_id) == actor_id_) {
        Node node(8 - idx / 9, idx % 9);
        auto event = std::make_shared<EventData_RequestActorMove>(actor_id, node);
        if (!event_manager_->queue_event(event)) {
            qDebug() << "failed to queue MoveActor event";
        }
    }
}

void GameView::on_wall_dropped(int actor_id, int wo, int row, int column)
{
    if (static_cast<ActorId>(actor_id) != actor_id_) {
        return;
    }

    auto rwo = static_cast<WallEnumClass::WallOrientation>(wo);
    Wall wall(rwo, Node(row, column), 2);

    qDebug() << "player" << actor_id << "adding"
             << (rwo == WallEnumClass::WallOrientation::WO_Vertical ? "vertical" : "horizontal")
             << "wall at" << row << ":" << column;

    auto event = std::make_shared<EventData_RequestSetWall>(actor_id, wall);
    if (!event_manager_->queue_event(event)) {
        qDebug() << "failed to queue RequestSetWall event";
    }
}

void GameView::button_back_clicked()
{
    auto game_terminated_event = std::make_shared<EventData_GameTerminated>();
    event_manager_->queue_event(game_terminated_event);

    auto event = std::make_shared<EventData_Options>();
    if (!event_manager_->queue_event(event)) {
        qDebug() << "failed to queue MainMenu event";
    }
}

QObject *GameView::find_object_by_name(const char *name) const
{
    return qroot_->findChild<QObject*>(name);
}

void GameView::connect_objects()
{
    qboard_ = find_object_by_name("boardFrame");
    if (!qboard_) {
        throw qml_missing_element_error();
    }

    qrecorder_ = find_object_by_name("moveRecorder");
    if (!qrecorder_) {
        throw qml_missing_element_error();
    }

    if (!QObject::connect(
            qboard_, SIGNAL(pawnDropped(int, int)),
            this, SLOT(on_pawn_dropped(int, int)))) {
        throw qml_connect_error();
    }
    if (!QObject::connect(
            qboard_, SIGNAL(wallDropped(int, int, int, int)),
            this, SLOT(on_wall_dropped(int, int, int, int)))) {
        throw qml_connect_error();
    }
}
