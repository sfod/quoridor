#include "start_game_state.hpp"
#include "game_state.hpp"
#include "main_menu_state.hpp"

namespace Quoridor {

StartGameState::StartGameState() : players_()
{
}

StartGameState::~StartGameState()
{
}

void StartGameState::handle_events(StateManager *stm,
        std::shared_ptr<UI::UIImpl> ui)
{
    UI::Event ev;
    if (ui->poll_event(&ev)) {
        switch (ev) {
        case UI::kEnter: {
            std::shared_ptr<Quoridor::IState> menu_state(new GameState());
            stm->change_state(std::shared_ptr<IState>(menu_state));
            }
            break;
        case UI::kEsc: {
            std::shared_ptr<Quoridor::IState> menu_state(new MainMenuState());
            stm->change_state(std::shared_ptr<IState>(menu_state));
            }
            break;
        default:
            break;
        }
    }
}

void StartGameState::update()
{
}

void StartGameState::draw(std::shared_ptr<UI::UIImpl> /* ui */)
{
}

void StartGameState::change_state()
{
}

void StartGameState::set_player(int i, std::shared_ptr<IPlayer> player)
{
    players_[i] = player;
}


}  /* namespace Quoridor */
