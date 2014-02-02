#include "state_manager.hpp"

namespace Quoridor {

StateManager::StateManager() : states_(), ui_(), is_running_(true)
{
}

StateManager::~StateManager()
{
}

void StateManager::create_ui(UI::UIFactory &uif, const std::string &ui_type)
{
    ui_ = uif.make_ui_impl(ui_type);
}

void StateManager::change_state(std::shared_ptr<IState> state)
{
    if (!states_.empty()) {
        states_.pop_back();
    }
    states_.push_back(state);
}

void StateManager::handle_events()
{
    states_.back()->handle_events();
}

void StateManager::update()
{
    states_.back()->update();
}

void StateManager::draw()
{
    states_.back()->draw();
}

}  /* namespace Quoridor */
