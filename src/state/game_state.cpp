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
        const std::vector<std::string> &player_types) : stm_(stm), anim_(),
    board_(new Board(9)), pf_(), players_(), pawn_list_(), cur_pawn_(),
    drag_list_(), pawn_path_(), added_wall_(0, 0, 0, 0), wall_idx_(0),
    status_(kWaitingForMove)
{
    lg.add_attribute("Tag", blattrs::constant<std::string>("game"));

    CEGUI::ImageManager::getSingleton().loadImageset("pawn.imageset");
    CEGUI::ImageManager::getSingleton().loadImageset("board.imageset");
    CEGUI::ImageManager::getSingleton().loadImageset("wall.imageset");
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
    switch_cur_pawn_();

    if (drag_list_.count(cur_pawn_)) {
        drag_list_[cur_pawn_]->enable_drag();
    }
}

GameState::~GameState()
{
}

void GameState::update()
{
    switch (status_) {
    case kWaitingForMove:
        make_move_();
        break;
    case kPerformedMove:
        status_ = kWaitingForMove;
        if (is_finished_()) {
            BOOST_LOG_SEV(lg, boost::log::trivial::info) << cur_pawn_->color()
                << " win";
            status_ = kFinished;
        }
        else {
            switch_cur_pawn_();
        }
        break;
    case kNeedPawnRedraw:
        redraw_pawn_();
        status_ = kWaitingForAnimationEnd;
        break;
    case kNeedDrawWall:
        draw_wall_();
        status_ = kPerformedMove;
        break;
    case kWaitingForAnimationEnd:
        break;
    case kFinished:
        break;
    default:
        break;
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

    board_win->subscribeEvent(
            CEGUI_Ext::DraggableWindow::EventDraggableWindowDropped,
            CEGUI::Event::Subscriber(&GameState::handle_pawn_dropped_, this));

    CEGUI::Window *drag_win;
    for (auto pawn : pawn_list_) {
        if (players_[pawn]->is_interactive()) {
            drag_win = new CEGUI_Ext::DraggableWindow("DefaultWindow", pawn->color());
            drag_list_[pawn] = static_cast<CEGUI_Ext::DraggableWindow*>(drag_win);
            drag_list_[pawn]->disable_drag();
        }
        else {
            drag_win = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", pawn->color());
            drag_win->subscribeEvent(CEGUI::AnimationInstance::EventAnimationEnded, CEGUI::Event::Subscriber(&GameState::handle_end_anim_, this));
        }

        drag_win->setSize(CEGUI::USize({0.1, 0}, {0.1, 0}));
        Pos pos = board_->pawn_node(pawn);
        float x_coord = 0.1111 * pos.col();
        float y_coord = 0.1111 * (8 - pos.row());
        drag_win->setPosition(CEGUI::UVector2({x_coord, 2}, {y_coord, 2}));

        auto pawn_win = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("pawn_anim.layout");
        drag_win->addChild(pawn_win);
        board_win->addChild(drag_win);
    }
}

void GameState::redraw_pawn_()
{
    Pos old_pos = pawn_path_[0];
    Pos new_pos = pawn_path_[1];

    pawn_path_.clear();

    float old_x_coord = 0.1111 * old_pos.col();
    float old_y_coord = 0.1111 * (8 - old_pos.row());
    float new_x_coord = 0.1111 * new_pos.col();
    float new_y_coord = 0.1111 * (8 - new_pos.row());

    CEGUI::Affector *affector = anim_->createAffector("Position", "UVector2");
    affector->setApplicationMethod(CEGUI::Affector::AM_Absolute);
    affector->createKeyFrame(0.0, "{{" + std::to_string(old_x_coord)
            + ", 2.0}, { " + std::to_string(old_y_coord) + ", 2.0}}");
    affector->createKeyFrame(0.5, "{{" + std::to_string(new_x_coord)
            + ", 2.0}, { " + std::to_string(new_y_coord) + ", 2.0}}");

    auto pawn_win = win_->getChild("boardWindow/" + cur_pawn_->color());
    CEGUI::AnimationInstance *instance = CEGUI::AnimationManager::
            getSingleton().instantiateAnimation(anim_.get());
    instance->setTargetWindow(pawn_win);
    instance->start();
}

void GameState::draw_wall_()
{
    auto board_win = static_cast<CEGUI::DefaultWindow*>(win_->getChild("boardWindow"));

    if (added_wall_.orientation() == 0) {
        for (int i = 0; i < added_wall_.cnt(); ++i) {
            float x = 0.1111 * (added_wall_.start_pos() + i);
            float y = 0.1111 * ( 8 - added_wall_.line());
            auto wall_win = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("horizontal_wall.layout");
            wall_win->setPosition(CEGUI::UVector2({x, 0}, {y, -2}));
            wall_win->setName("wallWindow" + std::to_string(wall_idx_));
            ++wall_idx_;
            board_win->addChild(wall_win);
            BOOST_LOG_SEV(lg, boost::log::trivial::info) << "added horizontal wall at "
                << x << ":" << y;
        }
    }
    else {
        for (int i = 0; i < added_wall_.cnt(); ++i) {
            float x = 0.1111 * (added_wall_.line());
            float y = 0.1111 * ( 8 - added_wall_.start_pos() - i);
            auto wall_win = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("vertical_wall.layout");
            wall_win->setPosition(CEGUI::UVector2({x, -2}, {y, 0}));
            wall_win->setName("wallWindow" + std::to_string(wall_idx_));
            ++wall_idx_;
            board_win->addChild(wall_win);
            BOOST_LOG_SEV(lg, boost::log::trivial::info) << "added vertiacl wall at "
                << x << ":" << y;
        }
    }
}

void GameState::switch_cur_pawn_()
{
    if (drag_list_.count(cur_pawn_)) {
        drag_list_.at(cur_pawn_)->disable_drag();
    }

    auto it = pawn_list_.begin();
    for (;it != pawn_list_.end(); ++it) {
        if (*it == cur_pawn_) {
            break;
        }
    }

    if ((it == pawn_list_.end()) || (++it == pawn_list_.end())) {
        cur_pawn_ = pawn_list_[0];
    }
    else {
        cur_pawn_ = *it;
    }

    if (drag_list_.count(cur_pawn_)) {
        drag_list_.at(cur_pawn_)->enable_drag();
    }
}

void GameState::make_move_()
{
    IMove *move = NULL;

    // bot's turn
    if (!players_[cur_pawn_]->is_interactive()) {
        move = players_[cur_pawn_]->get_move();
    }
    // human's turn, handle it in one of event handlers
    else {
        return;
    }

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
                status_ = kNeedPawnRedraw;
            }
        }
        else if (WallMove *wall_move = dynamic_cast<WallMove*>(move)) {
            const Wall &wall = wall_move->wall();
            rc = board_->add_wall(wall);
            if (rc == 0) {
                added_wall_ = wall;
                status_ = kNeedDrawWall;
            }
        }
    }
}

bool GameState::is_finished_() const
{
    return board_->is_at_goal_node(cur_pawn_);
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
    status_ = kPerformedMove;
    return true;
}

bool GameState::handle_pawn_dropped_(const CEGUI::EventArgs &e)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "pawn was dropped!";

    auto de = static_cast<const CEGUI_Ext::DragEvent &>(e);
    CEGUI::Vector2f rel_pos = CEGUI::CoordConverter::asRelative(
            de.window()->getPosition() + de.pos(),
            {468, 468}  // @fixme get parent size
    );
    Pos node = normalize_pawn_pos_(rel_pos);

    BOOST_LOG_SEV(lg, boost::log::trivial::info) << de.window() << " position "
        << node.row() << ":" << node.col();

    float x_coord;
    float y_coord;

    int rc = board_->make_walking_move(cur_pawn_, node);
    if (rc == 0) {
        x_coord = 0.1111 * node.col();
        y_coord = 0.1111 * (8 - node.row());
        status_ = kPerformedMove;
    }
    else {
        Pos cur_node = board_->pawn_node(cur_pawn_);
        x_coord = 0.1111 * cur_node.col();
        y_coord = 0.1111 * (8 - cur_node.row());
    }

    de.window()->setPosition(CEGUI::UVector2({x_coord, 2}, {y_coord, 2}));

    return true;
}

Pos GameState::normalize_pawn_pos_(const CEGUI::Vector2f &rel_pos)
{
    return Pos(
            8 - static_cast<int>(rel_pos.d_y / 0.1111),
            static_cast<int>(rel_pos.d_x / 0.1111)
    );
}

}  /* namespace Quoridor */
