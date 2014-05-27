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
std::string GameState::name_("Game");

GameState::GameState(std::shared_ptr<StateManager> stm,
        const std::vector<std::string> &player_types) : stm_(stm), anim_(),
    board_(new Board(9)), pf_(), players_(), pawn_list_(), cur_pawn_(),
    drag_list_(), pawn_path_(), added_wall_(0, 0, 0, 0), wall_idx_(0),
    status_(kWaitingForMove), node_utils_(9, 52, 50)
{
    lg.add_attribute("Tag", blattrs::constant<std::string>("game"));

    init_gui_();
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

    set_pawns_();
    init_walls_();
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
    case kPreparingMove:
        pre_process_move_();
        status_ = kWaitingForMove;
        break;
    case kWaitingForMove:
        make_move_();
        break;
    case kPerformedMove:
        post_process_move_();
        if (is_finished_()) {
            BOOST_LOG_SEV(lg, boost::log::trivial::info) << cur_pawn_->color() << " win";
            status_ = kFinished;
        }
        else {
            switch_cur_pawn_();
            status_ = kPreparingMove;
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

void GameState::init_gui_()
{
    CEGUI::ImageManager::getSingleton().loadImageset("pawn.imageset");
    CEGUI::ImageManager::getSingleton().loadImageset("board.imageset");
    CEGUI::ImageManager::getSingleton().loadImageset("wall.imageset");

    win_ = std::shared_ptr<CEGUI::Window>(
            CEGUI::WindowManager::getSingleton().
                    loadLayoutFromFile("game.layout"),
            [=](CEGUI::Window *w) {
                CEGUI::WindowManager::getSingleton().destroyWindow(w);
            }
    );

    anim_ = std::shared_ptr<CEGUI::Animation>(
            CEGUI::AnimationManager::getSingleton().
                    createAnimation("movePawn"),
            [=](CEGUI::Animation *anim) {
                CEGUI::AnimationManager::getSingleton().destroyAnimation(anim);
            }
    );
    anim_->setDuration(0.5);
    anim_->setReplayMode(CEGUI::Animation::RM_Once);
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
        Node node = board_->pawn_node(pawn);
        CEGUI::UVector2 pos = node_utils_.node_to_pos(node);
        drag_win->setPosition(pos);

        auto pawn_win = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("pawn_anim.layout");
        drag_win->addChild(pawn_win);
        board_win->addChild(drag_win);
    }
}

void GameState::init_walls_()
{
    auto ws1 = static_cast<CEGUI::DefaultWindow*>(win_->getChild("boardWindow"));
    for (int i = 0; i < 10; ++i) {
        auto w1 = new CEGUI_Ext::DraggableWindow("DefaultWindow", "wall_1_" + std::to_string(i));
        w1->setPosition(CEGUI::UVector2({{(float) 0.1 * (float) i, -5}, {0, 4}}));
        w1->setSize(CEGUI::USize({{0.3, 0}, {0.9, 0}}));

        auto wall_img = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("vertical_wall.layout");
        wall_img->setName("vwall");
        wall_img->setPosition(CEGUI::UVector2({0.0, 0.0}, {0.0, 0.0}));
        w1->addChild(wall_img);

        ws1->addChild(w1);
    }
}

void GameState::redraw_pawn_()
{
    Node old_node = pawn_path_[0];
    Node new_node = pawn_path_[1];

    pawn_path_.clear();

    CEGUI::UVector2 old_pos = node_utils_.node_to_pos(old_node);
    CEGUI::UVector2 new_pos = node_utils_.node_to_pos(new_node);
    std::string old_pos_str = "{{" + std::to_string(old_pos.d_x.d_scale) + ", "
        + std::to_string(old_pos.d_x.d_offset) + "}, {"
        + std::to_string(old_pos.d_y.d_scale) + ", "
        + std::to_string(old_pos.d_y.d_offset)+ "}}";
    std::string new_pos_str = "{{" + std::to_string(new_pos.d_x.d_scale) + ", "
        + std::to_string(new_pos.d_x.d_offset) + "}, {"
        + std::to_string(new_pos.d_y.d_scale) + ", "
        + std::to_string(new_pos.d_y.d_offset)+ "}}";

    CEGUI::Affector *affector = anim_->createAffector("Position", "UVector2");
    affector->setApplicationMethod(CEGUI::Affector::AM_Absolute);
    affector->createKeyFrame(0.0, old_pos_str);
    affector->createKeyFrame(0.5, new_pos_str);

    auto pawn_win = win_->getChild("boardWindow/" + cur_pawn_->color());
    CEGUI::AnimationInstance *instance = CEGUI::AnimationManager::
            getSingleton().instantiateAnimation(anim_.get());
    instance->setTargetWindow(pawn_win);
    instance->start();
}

void GameState::draw_wall_()
{
    auto board_win = static_cast<CEGUI::DefaultWindow*>(win_->getChild("boardWindow"));
    Node node;
    CEGUI::UVector2 pos;

    if (added_wall_.orientation() == 0) {
        for (int i = 0; i < added_wall_.cnt(); ++i) {
            node.set_row(8 - added_wall_.line());
            node.set_col(added_wall_.start_pos() + i);
            pos = node_utils_.node_to_pos(node);
            pos.d_y.d_offset = -2.0;
            auto wall_win = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("horizontal_wall.layout");
            wall_win->setPosition(pos);
            wall_win->setName("wallWindow" + std::to_string(wall_idx_));
            ++wall_idx_;
            board_win->addChild(wall_win);
            BOOST_LOG_SEV(lg, boost::log::trivial::info) << "added horizontal wall at " << pos;
        }
    }
    else {
        for (int i = 0; i < added_wall_.cnt(); ++i) {
            node.set_row(8 - added_wall_.start_pos() - i);
            node.set_col(added_wall_.line());
            pos = node_utils_.node_to_pos(node);
            pos.d_x.d_offset = -2.0;
            auto wall_win = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("vertical_wall.layout");
            wall_win->setPosition(pos);
            wall_win->setName("wallWindow" + std::to_string(wall_idx_));
            ++wall_idx_;
            board_win->addChild(wall_win);
            BOOST_LOG_SEV(lg, boost::log::trivial::info) << "added vertiacl wall at " << pos;
        }
    }
}

void GameState::pre_process_move_()
{
    if (drag_list_.count(cur_pawn_)) {
        drag_list_.at(cur_pawn_)->enable_drag();
    }
}

void GameState::post_process_move_()
{
    if (drag_list_.count(cur_pawn_)) {
        drag_list_.at(cur_pawn_)->disable_drag();
    }
}

void GameState::switch_cur_pawn_()
{
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
        Node cur_node = board_->pawn_node(cur_pawn_);
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
            {568, 568}  // @fixme get parent size
    );
    Node node = normalize_pawn_pos_(rel_pos);

    BOOST_LOG_SEV(lg, boost::log::trivial::info) << de.window() << " position "
        << node.row() << ":" << node.col();

    CEGUI::UVector2 pos;

    int rc = board_->make_walking_move(cur_pawn_, node);
    if (rc == 0) {
        pos = node_utils_.node_to_pos(node);
        status_ = kPerformedMove;
    }
    else {
        Node cur_node = board_->pawn_node(cur_pawn_);
        pos = node_utils_.node_to_pos(cur_node);
    }

    de.window()->setPosition(pos);

    return true;
}

Node GameState::normalize_pawn_pos_(const CEGUI::Vector2f &rel_pos)
{
    CEGUI::UVector2 pos({rel_pos.d_x, 0.0}, {rel_pos.d_y, 0.0});
    return node_utils_.pos_to_node(pos);
}

}  /* namespace Quoridor */
