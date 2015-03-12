#include "main_menu_view.hpp"
#include <QDebug>
#include "events/event_data.hpp"
#include "events/event_manager.hpp"

MainMenuView::MainMenuView(QObject *qroot, QObject *qparent)
    : QtView(qparent), qroot_(qroot), actor_id_(-1)
{
}

MainMenuView::~MainMenuView()
{
    qDebug() << "destroying MainMenuView";
}

bool MainMenuView::init()
{
    if (!connect_button("buttonNewGame", SLOT(button_new_game_clicked()))) {
        return false;
    }
    if (!connect_button("buttonQuit", SLOT(button_quit_clicked()))) {
        return false;
    }
    return true;
}

void MainMenuView::on_msg()
{
}

void MainMenuView::on_update()
{
}

void MainMenuView::attach(ActorId actor_id)
{
    actor_id_ = actor_id;
}

void MainMenuView::button_new_game_clicked()
{
    auto event = std::make_shared<EventData_Options>();
    if (!EventManager::get()->queue_event(event)) {
        qDebug() << "failed to queue Options event";
    }
}

void MainMenuView::button_quit_clicked()
{
    auto event = std::make_shared<EventData_Quit>();
    if (!EventManager::get()->queue_event(event)) {
        qDebug() << "failed to queue Quit event";
    }
}

QObject *MainMenuView::find_object_by_name(const char *name) const
{
    return qroot_->findChild<QObject*>(name);
}
