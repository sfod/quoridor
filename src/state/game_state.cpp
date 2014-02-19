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
    repr_.resize(19);
    for (int i = 0; i < 19; ++i) {
        repr_[i].resize(19);
        for (int j = 0; j < 19; ++j) {
            if (i % 2 == 1) {
                repr_[i][j] = (j % 2 == 0 ? '|' : ' ');
            }
            else {
                repr_[i][j] = (j % 2 == 0 ? ' ' : '_');
            }
        }
    }

    for (size_t i = 0; i < players_.size(); ++i) {
        auto pawn = game_->pawn_list().at(i);
        pos_t pos = game_->pawn_pos(pawn);
        repr_[pos.row * 2 + 1][pos.col * 2 + 1] = pawn->color()[0];
    }
}

void GameState::redraw_pawn(char p, const pos_t &old_pos, const pos_t &new_pos) const
{
    repr_[old_pos.row * 2 + 1][old_pos.col * 2 + 1] = ' ';
    repr_[new_pos.row * 2 + 1][new_pos.col * 2 + 1] = p;
}

void GameState::draw_wall(const Wall &wall) const
{
    if (wall.orientation() == 0) {
        for (int i = 0; i < wall.cnt(); ++i) {
            repr_[wall.line() * 2 + 2][(wall.start_pos() + i) * 2 + 1] = '=';
        }
    }
    else {
        for (int i = 0; i < wall.cnt(); ++i) {
            repr_[(wall.start_pos() + i) * 2 + 1][wall.line() * 2 + 2] = '$';
        }
    }
}

}  /* namespace Quoridor */
