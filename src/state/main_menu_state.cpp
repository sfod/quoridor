#include "main_menu_state.hpp"
#include "game_state.hpp"

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
        case UI::kEnter: {
            std::shared_ptr<Quoridor::IState> game_state(new Quoridor::GameState());
            stm->change_state(std::shared_ptr<Quoridor::IState>(game_state));
            }
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

void MainMenuState::draw(std::shared_ptr<UI::UIImpl> /* ui */)
{
}

void MainMenuState::change_state()
{
}

}  /* namespace Quoridor */
