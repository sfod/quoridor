#pragma once

#include <QObject>
#include "qt_view.hpp"
#include "events/event_manager.hpp"
#include "graph/wall.hpp"

class GameView : public QtView
{
    Q_OBJECT

public:
    explicit GameView(QObject *qroot, bool is_main, QObject *qparent = 0);
    virtual ~GameView();

    bool init();
    virtual void on_msg();
    virtual void on_update();
    virtual void attach(ActorId actor_id);

    void new_actor_delegate(const std::shared_ptr<EventData> &event);
    void move_actor_delegate(const std::shared_ptr<EventData> &event);
    void set_actor_possible_moves_delegate(const std::shared_ptr<EventData> &event);
    void set_wall_delegate(const std::shared_ptr<EventData> &event);
    void set_active_delegate(const std::shared_ptr<EventData> &event);
    void game_finished_delegate(const std::shared_ptr<EventData> &event);

public slots:
    void on_pawn_dropped(int actor_id, int idx);
    void on_wall_dropped(int actor_id, int wo, int row, int column);
    void button_back_clicked();

private:
    virtual QObject *find_object_by_name(const char *name) const override;
    bool connect_board();

private:
    std::list<bs2::connection> conn_list_;
    QObject *qroot_;
    QObject *qboard_;
    QObject *qbutton_;
    ActorId actor_id_;
    bool is_main_;
};
