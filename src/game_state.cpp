#include "game_state.hpp"
#include "start_game_state.hpp"

namespace Quoridor {

GameState::GameState()
{
}

GameState::~GameState()
{
}

void GameState::handle_events(StateManager *stm, std::shared_ptr<UI::UIImpl> ui)
{
    UI::Event ev;
    if (ui->poll_event(&ev)) {
        switch (ev) {
        case UI::kEnter:
            break;
        case UI::kEsc: {
            std::shared_ptr<Quoridor::IState> menu_state(new StartGameState());
            stm->change_state(std::shared_ptr<IState>(menu_state));
            }
            break;
        default:
            break;
        }
    }
}

void GameState::update()
{
}

void GameState::draw()
{
}

void GameState::change_state()
{
}

}  /* namespace Quoridor */
