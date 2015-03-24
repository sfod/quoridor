#include "game_view.hpp"
#include <QQuickItem>
#include <QDebug>

GameView::GameView(QObject *qroot, bool is_main, QObject *qparent)
    : QtView(qparent), conn_list_(), qroot_(qroot), qboard_(), qbutton_(),
      actor_id_(-1), is_main_(is_main)
{
}

GameView::~GameView()
{
    qDebug() << "destroying PlayerView";
    for (auto conn : conn_list_) {
        conn.disconnect();
    }
}

bool GameView::init()
{
    if (!connect_board()) {
        return false;
    }

    if (is_main_ && !connect_button("buttonBackToOptions", SLOT(button_back_clicked()), &qbutton_)) {
        return false;
    }

    bs2::connection conn;
    conn = EventManager::get()->add_listener(
            boost::bind(&GameView::new_actor_delegate, this, _1),
            EventData_NewActor::event_type_);
    conn_list_.push_back(conn);

    conn = EventManager::get()->add_listener(
            boost::bind(&GameView::move_actor_delegate, this, _1),
            EventData_MoveActor::event_type_);
    conn_list_.push_back(conn);

    conn = EventManager::get()->add_listener(
            boost::bind(&GameView::set_wall_delegate, this, _1),
            EventData_SetWall::event_type_);
    conn_list_.push_back(conn);

    conn = EventManager::get()->add_listener(
            boost::bind(&GameView::set_actor_possible_moves_delegate, this, _1),
            EventData_SetActorPossibleMoves::event_type_);

    conn = EventManager::get()->add_listener(
            boost::bind(&GameView::set_active_delegate, this, _1),
            EventData_SetActorActive::event_type_);
    conn_list_.push_back(conn);

    conn = EventManager::get()->add_listener(
            boost::bind(&GameView::game_finished_delegate, this, _1),
            EventData_GameFinished::event_type_);
    conn_list_.push_back(conn);

    if (is_main_) {
        QMetaObject::invokeMethod(qboard_, "init");
    }

    return true;
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
        QMetaObject::invokeMethod(qboard_, "addPawn",
                Q_ARG(QVariant, static_cast<int>(new_actor_event->actor_id())));
    }
}

void GameView::move_actor_delegate(const std::shared_ptr<EventData> &event)
{
    auto move_event = std::dynamic_pointer_cast<EventData_MoveActor>(event);
    if (is_main_) {
        const Node &node = move_event->node();
        int idx = (8 - node.row()) * 9 + node.col();

        qDebug() << "move actor delegate";

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

void GameView::game_finished_delegate(const std::shared_ptr<EventData> &/*event*/)
{
//    auto game_finished_event = std::dynamic_pointer_cast<EventData_GameFinished>(event);
    qDebug() << "finishing game!";
    QMetaObject::invokeMethod(qboard_, "finishGame");
}

void GameView::on_pawn_dropped(int actor_id, int idx)
{
    if (static_cast<ActorId>(actor_id) == actor_id_) {
        Node node(8 - idx / 9, idx % 9);
        auto event = std::make_shared<EventData_RequestActorMove>(actor_id, node);
        if (!EventManager::get()->queue_event(event)) {
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
    if (!EventManager::get()->queue_event(event)) {
        qDebug() << "failed to queue RequestSetWall event";
    }
}

void GameView::button_back_clicked()
{
    auto game_terminated_event = std::make_shared<EventData_GameTerminated>();
    EventManager::get()->queue_event(game_terminated_event);

    auto event = std::make_shared<EventData_Options>();
    if (!EventManager::get()->queue_event(event)) {
        qDebug() << "failed to queue MainMenu event";
    }
}

QObject *GameView::find_object_by_name(const char *name) const
{
    return qroot_->findChild<QObject*>(name);
}

bool GameView::connect_board()
{
    qboard_ = find_object_by_name("board");
    if (qboard_ == NULL) {
        return false;
    }
    QObject::connect(
            qboard_, SIGNAL(pawnDropped(int, int)),
            this, SLOT(on_pawn_dropped(int, int))
    );
    QObject::connect(
            qboard_, SIGNAL(wallDropped(int, int, int, int)),
            this, SLOT(on_wall_dropped(int, int, int, int)));
    return true;
}
