#include "game_state.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>

#include "start_game_state.hpp"
#include "imove.hpp"
#include "walk_move.hpp"
#include "wall_move.hpp"
#include "exception.hpp"

namespace Quoridor {

static std::vector<std::string> colors = {"red", "green", "blue", "yellow"};

GameState::GameState(std::shared_ptr<UI::UI> ui,
        const std::vector<std::string> &player_types)
    : game_(new Game()), pf_(), players_(), cur_player_idx_(0), repr_(),
    is_running_(true)
{
    win_ = ui->create_window();

    if ((player_types.size() != 2) && (player_types.size() != 4)) {
        throw Exception("Invalid number of players");
    }

    int i = 0;
    for (auto player_type : player_types) {
        std::shared_ptr<Pawn> pawn(new Pawn(colors[i]));
        game_->add_pawn(pawn);
        players_.push_back(pf_.make_player(player_type, game_->board(), pawn));
        ++i;
    }

    init_board_repr();
}

GameState::~GameState()
{
}

void GameState::handle_events(StateManager *stm)
{
    std::shared_ptr<UI::UI> ui = stm->ui();
    UI::Event ev;
    IMove *move = NULL;

    if (!is_running_) {
            boost::this_thread::sleep(boost::posix_time::seconds(3));
            std::shared_ptr<IState> start_game_state(new StartGameState(ui));
            stm->change_state(std::shared_ptr<IState>(start_game_state));
            return;
    }

    if (!players_[cur_player_idx_]->is_interactive()) {
        move = players_[cur_player_idx_]->get_move();
    }
    else if (ui->poll_event(&ev)) {
        switch (ev) {
        case UI::kEsc: {
            std::shared_ptr<IState> start_game_state(new StartGameState(ui));
            stm->change_state(std::shared_ptr<IState>(start_game_state));
            break;
        }
        default:
            break;
        }
    }

    if (move != NULL) {
        auto pawn = game_->pawn_list().at(cur_player_idx_);
        pos_t prev_pos = game_->pawn_pos(pawn);

        int rc = game_->make_move(move, pawn);
        if (rc == 0) {
            if (dynamic_cast<WalkMove *>(move)) {
                pos_t pos = game_->pawn_pos(pawn);
                redraw_pawn(pawn->color()[0], prev_pos, pos);
            }
            else if (WallMove *wall_move = dynamic_cast<WallMove *>(move)) {
                const Wall &wall = wall_move->wall();
                draw_wall(wall);
            }
            cur_player_idx_ = (cur_player_idx_ + 1) % players_.size();
        }
        // player should continue turn
        else if (rc == -2) {
        }
        // invalid move, player should make another move
        else if (rc == -1) {
        }
        // not possible
        else {
            throw Exception("unknown return code from make_move :"
                    + boost::lexical_cast<std::string>(rc));
        }

        if (game_->is_win(pawn)) {
            win_->print_message(boost::lexical_cast<std::string>(pawn->color()) + " win");
            is_running_ = false;
        }
    }

    boost::this_thread::sleep(boost::posix_time::milliseconds(250));
}

void GameState::update()
{
    win_->draw_board(repr_);
}

void GameState::draw()
{
    win_->draw();
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
