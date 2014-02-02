#include "main_menu_state.hpp"

namespace Quoridor {

MainMenuState::MainMenuState()
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::handle_events(StateManager *stm, std::shared_ptr<UI::UIImpl> ui)
{
    UI::Event ev;
    if (ui->poll_event(&ev)) {
        switch (ev) {
        case UI::kEnter:
            break;
        case UI::kEsc:
            stm->stop();
            break;
        default:
            break;
        }
    }
}

void MainMenuState::update()
{
}

void MainMenuState::draw()
{
}

void MainMenuState::change_state()
{
}

}  /* namespace Quoridor */
