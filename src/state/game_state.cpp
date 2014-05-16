#include "game_state.hpp"

#include "start_game_state.hpp"
#include "imove.hpp"
#include "walk_move.hpp"
#include "wall_move.hpp"
#include "logger.hpp"
#include "exception.hpp"


static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

namespace Quoridor {

static std::vector<std::string> colors = {"red", "green", "blue", "yellow"};
std::string GameState::name_("Start Game");

GameState::GameState(std::shared_ptr<StateManager> stm,
        const std::vector<std::string> &player_types) : stm_(stm),
    board_(new Board(9)), pf_(), players_(), pawn_list_(), cur_pawn_(),
    repr_(), is_running_(true)
{
    CEGUI::ImageManager::getSingleton().loadImageset("pawn.imageset");
    win_ = std::shared_ptr<CEGUI::Window>(
            CEGUI::WindowManager::getSingleton().
                    loadLayoutFromFile("game.layout"),
            [=](CEGUI::Window *w) {
                BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "removing window " << w;
                CEGUI::WindowManager::getSingleton().destroyWindow(w);
            }
    );

    if ((player_types.size() != 2) && (player_types.size() != 4)) {
        throw Exception("Invalid number of players");
    }

    int i = 0;
    for (auto player_type : player_types) {
        BOOST_LOG_SEV(lg, boost::log::trivial::info) << "adding player "
            << player_type;
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

void GameState::update()
{
    IMove *move;

    if (!is_running_) {
        return;
    }

    // if (!players_[cur_pawn_]->is_interactive()) {
        move = players_[cur_pawn_]->get_move();
    // }
    if (move != NULL) {
        Pos cur_node = board_->pawn_node(cur_pawn_);
        int rc;

        if (WalkMove *walk_move = dynamic_cast<WalkMove*>(move)) {
            BOOST_LOG_SEV(lg, boost::log::trivial::info) << cur_pawn_->color()
                << " move: " << walk_move->node().row() << ":"
                << walk_move->node().col();
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
            BOOST_LOG_SEV(lg, boost::log::trivial::info) << cur_pawn_->color()
                << " win";
            is_running_ = false;
        }
        else if (rc == 0) {
            cur_pawn_ = next_pawn();
        }

    }

}

std::shared_ptr<CEGUI::Window> GameState::window() const
{
    return win_;
}

const std::string &GameState::name() const
{
    return name_;
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
