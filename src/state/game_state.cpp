#include "game_state.hpp"
#include "start_game_state.hpp"

namespace Quoridor {

GameState::GameState(std::shared_ptr<UI::UI> ui,
        const std::vector<std::string> &player_types)
    : game_(), pf_(), players_(), repr_()
{
    win_ = ui->create_window();

    for (auto player_type : player_types) {
        players_.push_back(pf_.make_player(player_type));
    }
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
