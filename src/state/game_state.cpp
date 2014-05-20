#include "game_state.hpp"

#include "dragged_window.hpp"
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
        const std::vector<std::string> &player_types) : stm_(stm), anim_(),
    board_(new Board(9)), pf_(), players_(), pawn_list_(), cur_pawn_(),
    repr_(), is_finished_(false), is_wait_move_(true), need_redraw_(false),
    pawn_path_()
{
    CEGUI::ImageManager::getSingleton().loadImageset("pawn.imageset");
    CEGUI::ImageManager::getSingleton().loadImageset("board.imageset");
    win_ = std::shared_ptr<CEGUI::Window>(
            CEGUI::WindowManager::getSingleton().
                    loadLayoutFromFile("game.layout"),
            [=](CEGUI::Window *w) {
                BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "removing window " << w;
                CEGUI::WindowManager::getSingleton().destroyWindow(w);
            }
    );

    subscribe_for_events_();

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

    anim_ = std::shared_ptr<CEGUI::Animation>(
            CEGUI::AnimationManager::getSingleton().
                    createAnimation("movePawn"),
            [=](CEGUI::Animation *anim) {
                CEGUI::AnimationManager::getSingleton().destroyAnimation(anim);
            }
    );
    anim_->setDuration(0.5);
    anim_->setReplayMode(CEGUI::Animation::RM_Once);

    set_pawns_();

    cur_pawn_ = pawn_list_[0];
}

GameState::~GameState()
{
}

void GameState::update()
{
    IMove *move;

    if (need_redraw_) {
        redraw_pawn(cur_pawn_);
        need_redraw_ = false;
        return;
    }

    if (!is_wait_move_) {
        return;
    }

    if (is_finished_) {
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
                << " move: " << cur_node.row() << ":" << cur_node.col()
                << " -> " << walk_move->node().row() << ":"
                << walk_move->node().col();

            rc = board_->make_walking_move(cur_pawn_, walk_move->node());
            if (rc == 0) {
                pawn_path_.push_back(cur_node);
                pawn_path_.push_back(board_->pawn_node(cur_pawn_));
                is_wait_move_ = false;
                need_redraw_ = true;
            }
        }
        else if (WallMove *wall_move = dynamic_cast<WallMove*>(move)) {
            const Wall &wall = wall_move->wall();
            rc = board_->add_wall(wall);
            if (rc == 0) {
            }
        }

        if (board_->is_at_goal_node(cur_pawn_)) {
            BOOST_LOG_SEV(lg, boost::log::trivial::info) << cur_pawn_->color()
                << " win";
            is_finished_ = true;
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

void GameState::set_pawns_()
{
    auto board_win = static_cast<CEGUI::DefaultWindow*>(win_->getChild("boardWindow"));

    for (auto pawn : pawn_list_) {
        CEGUI::DraggedWindow *drag_win = new CEGUI::DraggedWindow("DefaultWindow", pawn->color());
        drag_win->setSize(CEGUI::USize({0.1, 0}, {0.1, 0}));
        Pos pos = board_->pawn_node(pawn);
        float x_coord = 0.1111 * pos.col();
        float y_coord = 0.1111 * pos.row();
        drag_win->setPosition(CEGUI::UVector2({x_coord, 2}, {y_coord, 2}));
        drag_win->subscribeEvent(CEGUI::AnimationInstance::EventAnimationEnded, CEGUI::Event::Subscriber(&GameState::handle_end_anim_, this));

        auto pawn_win = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("pawn_anim.layout");
        drag_win->addChild(pawn_win);
        board_win->addChild(drag_win);
    }
}

void GameState::subscribe_for_events_()
{
    win_->getChild("back")->subscribeEvent(
            CEGUI::Window::EventMouseClick,
            CEGUI::Event::Subscriber(
                    &GameState::handle_back_, this
            )
    );
}

bool GameState::handle_back_(const CEGUI::EventArgs &/* e */)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "returning to start game menu";
    stm_->change_state(std::shared_ptr<IState>(new StartGameState(stm_)));
    return true;
}

bool GameState::handle_end_anim_(const CEGUI::EventArgs &/* e */)
{
    is_wait_move_ = true;
    return true;
}

void GameState::redraw_pawn(std::shared_ptr<Pawn> pawn)
{
    Pos old_pos = pawn_path_[0];
    Pos new_pos = pawn_path_[1];

    pawn_path_.clear();

    float old_x_coord = 0.1111 * old_pos.col();
    float old_y_coord = 0.1111 * old_pos.row();
    float new_x_coord = 0.1111 * new_pos.col();
    float new_y_coord = 0.1111 * new_pos.row();

    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "pawn path: "
        << old_x_coord << ":" << old_y_coord << " --> " << new_x_coord
        << ":" << new_y_coord;

    CEGUI::Affector *affector = anim_->createAffector("Position", "UVector2");
    affector->setApplicationMethod(CEGUI::Affector::AM_Absolute);
    affector->createKeyFrame(0.0, "{{" + std::to_string(old_x_coord)
            + ", 2.0}, { " + std::to_string(old_y_coord) + ", 2.0}}");
    affector->createKeyFrame(0.5, "{{" + std::to_string(new_x_coord)
            + ", 2.0}, { " + std::to_string(new_y_coord) + ", 2.0}}");

    auto pawn_win = win_->getChild("boardWindow/" + pawn->color());
    CEGUI::AnimationInstance *instance = CEGUI::AnimationManager::
            getSingleton().instantiateAnimation(anim_.get());
    instance->setTargetWindow(pawn_win);
    instance->start();
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
