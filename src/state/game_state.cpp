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
    : board_(new Board(9, 9)), pf_(), players_(), pawn_list_(), cur_pawn_(),
    repr_(), is_running_(true)
{
    win_ = ui->create_window();

    if ((player_types.size() != 2) && (player_types.size() != 4)) {
        throw Exception("Invalid number of players");
    }

    int i = 0;
    for (auto player_type : player_types) {
        std::shared_ptr<Pawn> pawn(new Pawn(colors[i]));
        board_->add_pawn(pawn);
        players_[pawn] = pf_.make_player(player_type, board_, pawn);
        pawn_list_.push_back(pawn);
        ++i;
    }

    cur_pawn_ = pawn_list_[0];

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
        win_->print_message(boost::lexical_cast<std::string>(cur_pawn_->color()) + " win");
        win_->draw();
        boost::this_thread::sleep(boost::posix_time::seconds(2));
        std::shared_ptr<IState> start_game_state(new StartGameState(ui));
        stm->change_state(std::shared_ptr<IState>(start_game_state));
        return;
    }

    if (!players_[cur_pawn_]->is_interactive()) {
        move = players_[cur_pawn_]->get_move();
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
        Pos cur_node = board_->pawn_node(cur_pawn_);
        int rc;

        if (WalkMove *walk_move = dynamic_cast<WalkMove*>(move)) {
            rc = board_->make_walking_move(cur_pawn_, walk_move->node());
            if (rc == 0) {
                Pos goal_node = board_->pawn_node(cur_pawn_);
                redraw_pawn(cur_pawn_->color()[0], cur_node, goal_node);
            }
        }
        else if (WallMove *wall_move = dynamic_cast<WallMove*>(move)) {
            const Wall &wall = wall_move->wall();
            rc = board_->add_wall(wall);
            if (rc == 0) {
                draw_wall(wall);
            }
        }

        if (board_->is_at_goal_node(cur_pawn_)) {
            is_running_ = false;
        }
        else if (rc == 0) {
            cur_pawn_ = next_pawn();
        }

    }

    boost::this_thread::sleep(boost::posix_time::milliseconds(200));
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

    for (auto pawn : pawn_list_) {
        Pos pos = board_->pawn_node(pawn);
        repr_[pos.row() * 2 + 1][pos.col() * 2 + 1] = pawn->color()[0];
    }
}

void GameState::redraw_pawn(char p, const Pos &old_pos, const Pos &new_pos) const
{
    repr_[old_pos.row() * 2 + 1][old_pos.col() * 2 + 1] = ' ';
    repr_[new_pos.row() * 2 + 1][new_pos.col() * 2 + 1] = p;
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

std::shared_ptr<Pawn> GameState::next_pawn() const
{
    auto it = pawn_list_.begin();
    for (;it != pawn_list_.end(); ++it) {
        if (*it == cur_pawn_) {
            break;
        }
    }
    if (++it == pawn_list_.end()) {
        return pawn_list_[0];
    }
    else {
        return *it;
    }
}

}  /* namespace Quoridor */
