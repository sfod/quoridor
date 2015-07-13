#include "main_menu_view.hpp"
#include "events/event_data.hpp"
#include "events/event_manager.hpp"
#include "exceptions/exception.hpp"


MainMenuView::MainMenuView(QObject *qroot, QObject *qparent)
    : QtView(qparent), qroot_(qroot), actor_id_(-1)
{
    connect_button("buttonNewGame", SLOT(button_new_game_clicked()));
    connect_button("buttonQuit", SLOT(button_quit_clicked()));
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

    }
}

void MainMenuView::button_quit_clicked()
{
    auto event = std::make_shared<EventData_Quit>();
    if (!EventManager::get()->queue_event(event)) {
    }
}

QObject *MainMenuView::find_object_by_name(const char *name) const
{
    return qroot_->findChild<QObject*>(name);
}
