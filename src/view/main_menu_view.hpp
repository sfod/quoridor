#pragma once

#include <list>
#include <QObject>
#include "qt_view.hpp"
#include "events/event_manager.hpp"

class MainMenuView : public QtView {
    Q_OBJECT

public:
    MainMenuView(QObject *qroot, const std::shared_ptr<EventManager> &event_manager);

    virtual void on_msg() override;
    virtual void on_update() override;
    virtual void attach(ActorId actor_id);

public slots:
    void button_new_game_clicked();
    void button_quit_clicked();

private:
    virtual QObject *find_object_by_name(const char *name) const override;

private:
    QObject *qroot_;
    ActorId actor_id_;
    std::shared_ptr<EventManager> event_manager_;
};
