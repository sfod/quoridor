#include "game_state.hpp"
#include "start_game_state.hpp"

namespace Quoridor {

GameState::GameState()
{
}

GameState::~GameState()
{
}

void GameState::handle_events(StateManager *stm)
{
    std::shared_ptr<UI::UI> ui = stm->ui();
    UI::Event ev;

    if (ui->poll_event(&ev)) {
        switch (ev) {
        case UI::kEnter:
            break;
        case UI::kEsc: {
            std::shared_ptr<IState> start_game_state(new StartGameState(ui));
            stm->change_state(std::shared_ptr<IState>(start_game_state));
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

void GameState::init_board_repr() const
{
}

}  /* namespace Quoridor */
