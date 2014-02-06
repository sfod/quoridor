#include "main_menu_state.hpp"
#include "start_game_state.hpp"

namespace Quoridor {

MainMenuState::MainMenuState(std::shared_ptr<UI::UI> ui)
    : win_(ui->create_window())
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::handle_events(StateManager *stm)
{
    std::shared_ptr<UI::UI> ui = stm->ui();
    UI::Event ev;

    if (ui->poll_event(&ev)) {
        switch (ev) {
        case UI::kEnter: {
            std::shared_ptr<IState> start_game_state(new StartGameState(ui));
            stm->change_state(std::shared_ptr<IState>(start_game_state));
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

void MainMenuState::draw()
{
}

void MainMenuState::change_state()
{
}

}  /* namespace Quoridor */
