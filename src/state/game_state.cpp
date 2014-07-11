#include "game_state.hpp"

#include "start_game_state.hpp"
#include "logger.hpp"
#include "exception.hpp"

static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

namespace Quoridor {

static std::vector<std::string> colors = {"red", "green", "blue", "yellow"};
std::string GameState::name_("Game State");

GameState::GameState(std::shared_ptr<StateManager> stm,
        const std::vector<std::string> &player_types) : stm_(stm), anim_(),
    game_(new Game(9, 9)), pf_(), players_(), pawn_list_(), cur_pawn_(),
    drag_list_(), pawn_wins_(), wall_wins_(), pawn_path_(),
    added_wall_(Wall::kInvalid, 0, 0, 0), wall_idx_(0),
    status_(kNeedMove),
    pos_utils_(9, 52, 50)
{
    lg.add_attribute("Tag", blattrs::constant<std::string>("game state"));

    init_gui_();
    subscribe_for_events_();

    if ((player_types.size() != 2) && (player_types.size() != 4)) {
        throw Exception("Invalid number of players");
    }

    for (size_t i = 0; i < player_types.size(); ++i) {
        std::shared_ptr<Pawn> pawn(new Pawn(colors[i]));
        pawn_list_.push_back(pawn);
    }

    try {
        game_->set_pawns(pawn_list_);
    }
    catch (Exception &e) {
        BOOST_LOG_ERROR(lg) << "failed to create game: " << e.what();
        throw;
    }

    int i = 0;
    for (auto player_type : player_types) {
        BOOST_LOG_INFO(lg) << "adding player " << player_type << " (" << colors[i] << ")";
        players_[pawn_list_[i]] = pf_.make_player(player_type, game_, pawn_list_[i]);
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
        status_ = kNeedMove;
        break;
    case kNeedMove:
        status_ = kWaitingForMove;
        make_move_();
        break;
    case kWaitingForMove:
        break;
    case kPerformedMove:
        post_process_move_();
        if (is_finished_()) {
            BOOST_LOG_INFO(lg) << cur_pawn_->color() << " win";
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
    CEGUI::ImageManager::getSingleton().loadImageset("wall_repr.imageset");

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
            CEGUI::Event::Subscriber(&GameState::handle_window_dropped_, this));

    CEGUI::Window *drag_win;
    for (auto pawn_data : game_->pawn_data_list()) {
        if (players_[pawn_data.pawn]->is_interactive()) {
            drag_win = new CEGUI_Ext::DraggableWindow("DefaultWindow", pawn_data.pawn->color());
            drag_list_[pawn_data.pawn] = static_cast<CEGUI_Ext::DraggableWindow*>(drag_win);
            drag_list_[pawn_data.pawn]->disable_drag();
            pawn_wins_[drag_win] = pawn_data.pawn;
        }
        else {
            drag_win = CEGUI::WindowManager::getSingleton().
                createWindow("DefaultWindow", pawn_data.pawn->color());
            drag_win->subscribeEvent(
                    CEGUI::AnimationInstance::EventAnimationEnded,
                    CEGUI::Event::Subscriber(&GameState::handle_end_anim_, this));
        }

        drag_win->setSize(CEGUI::USize({0.1, 0}, {0.1, 0}));
        CEGUI::UVector2 pos = pos_utils_.node_to_pos(pawn_data.node);
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
        w1->setPosition(CEGUI::UVector2({{0.0f, 25.0f * i + 4.0f}, {0.0f, 4.0f}}));
        w1->setSize(CEGUI::USize({{0.0, 21.0}, {0.0, 42.0}}));

        auto wall_img = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("wall_repr.layout");
        w1->addChild(wall_img);
        ws1->addChild(w1);

        wall_wins_[w1] = 1;
    }
}

void GameState::redraw_pawn_()
{
    Node old_node = pawn_path_[0];
    Node new_node = pawn_path_[1];

    pawn_path_.clear();

    CEGUI::UVector2 old_pos = pos_utils_.node_to_pos(old_node);
    CEGUI::UVector2 new_pos = pos_utils_.node_to_pos(new_node);
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

    if (added_wall_.orientation() == Wall::kHorizontal) {
        for (int i = 0; i < added_wall_.cnt(); ++i) {
            node.set_row(added_wall_.row() - 1);
            node.set_col(added_wall_.col() + i);
            pos = pos_utils_.node_to_pos(node);
            pos.d_y.d_offset = -2.0;
            auto wall_win = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("horizontal_wall.layout");
            wall_win->setPosition(pos);
            wall_win->setName("wallWindow" + std::to_string(wall_idx_));
            ++wall_idx_;
            board_win->addChild(wall_win);
        }
    }
    else if (added_wall_.orientation() == Wall::kVertical) {
        for (int i = 0; i < added_wall_.cnt(); ++i) {
            node.set_row(added_wall_.row() + i);
            node.set_col(added_wall_.col());
            pos = pos_utils_.node_to_pos(node);
            pos.d_x.d_offset = -2.0;
            auto wall_win = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("vertical_wall.layout");
            wall_win->setPosition(pos);
            wall_win->setName("wallWindow" + std::to_string(wall_idx_));
            ++wall_idx_;
            board_win->addChild(wall_win);
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
    game_->switch_pawn();
    cur_pawn_ = game_->cur_pawn_data().pawn;
}

void GameState::make_move_()
{
    // human's turn, handle it in one of event handlers
    if (players_[cur_pawn_]->is_interactive()) {
        return;
    }

    auto f = std::bind(&GameState::callback_move, this, std::placeholders::_1);
    players_[cur_pawn_]->get_move(f);
}

int GameState::move_pawn_(const Node &node)
{
    Node cur_node = game_->cur_pawn_data().node;
    int rc = game_->move_pawn(node);
    if (rc == 0) {
        BOOST_LOG_INFO(lg) << cur_pawn_->color() << ": " << cur_node << " -> "
            << node;
    }
    return rc;
}

int GameState::add_wall_(const Wall &wall)
{
    int rc = game_->add_wall(wall);
    if (rc == 0) {
        BOOST_LOG_INFO(lg) << cur_pawn_->color() << ": " << wall;
        added_wall_ = wall;
    }
    return rc;
}

bool GameState::is_finished_() const
{
    return game_->is_finished();
}

void GameState::callback_move(move_t move)
{
    BOOST_LOG_DEBUG(lg) << "callback_move called";

    if (move.which() == 0) {
        throw Exception("invalid move");
    }

    if (Node *node = boost::get<Node>(&move)) {
        Node cur_node = game_->cur_pawn_data().node;
        if (move_pawn_(*node) == 0) {
            pawn_path_.push_back(cur_node);
            pawn_path_.push_back(*node);
            status_ = kNeedPawnRedraw;
        }
    }
    else if (Wall *wall = boost::get<Wall>(&move)) {
        if (add_wall_(*wall) == 0) {
            status_ = kNeedDrawWall;
        }
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
    BOOST_LOG_DEBUG(lg) << "returning to start game menu";
    stm_->change_state(std::shared_ptr<IState>(new StartGameState(stm_)));
    return true;
}

bool GameState::handle_end_anim_(const CEGUI::EventArgs &/* e */)
{
    status_ = kPerformedMove;
    return true;
}

bool GameState::handle_window_dropped_(const CEGUI::EventArgs &e)
{
    auto de = static_cast<const CEGUI_Ext::DragEvent&>(e);
    if (pawn_wins_.count(de.window())) {
        return handle_pawn_dropped_(de);
    }
    else if (wall_wins_.count(de.window())) {
        return handle_wall_dropped_(de);
    }
    else {
        return false;
    }
}

bool GameState::handle_pawn_dropped_(const CEGUI_Ext::DragEvent &de)
{
    CEGUI::Vector2f rel_pos = CEGUI::CoordConverter::asRelative(
            de.window()->getPosition() + de.pos(),
            {568, 568}  // @fixme get parent size
    );
    Node node = normalize_pawn_pos_(rel_pos);

    CEGUI::UVector2 pos;
    int rc = move_pawn_(node);
    if (rc == 0) {
        pos = pos_utils_.node_to_pos(node);
        status_ = kPerformedMove;
    }
    else {
        Node cur_node = game_->cur_pawn_data().node;
        pos = pos_utils_.node_to_pos(cur_node);
    }

    de.window()->setPosition(pos);

    return true;
}

bool GameState::handle_wall_dropped_(const CEGUI_Ext::DragEvent &de)
{
    CEGUI::Vector2f rel_pos = CEGUI::CoordConverter::asRelative(
            de.window()->getPosition() + de.pos(),
            {568, 568}  // @fixme get parent size
    );
    Wall wall = normalize_wall_pos_(rel_pos);
    int rc = add_wall_(wall);
    if (rc == 0) {
        status_ = kNeedDrawWall;
        de.window()->setVisible(false);
    }

    return true;
}

Node GameState::normalize_pawn_pos_(const CEGUI::Vector2f &rel_pos)
{
    CEGUI::UVector2 pos({rel_pos.d_x, 0.0}, {rel_pos.d_y, 0.0});
    return pos_utils_.pos_to_node(pos);
}

Wall GameState::normalize_wall_pos_(const CEGUI::Vector2f &rel_pos)
{
    CEGUI::UVector2 pos({rel_pos.d_x, 0.0}, {rel_pos.d_y, 0.0});
    Wall wall = pos_utils_.pos_to_wall(pos, 2);
    return wall;
}

}  /* namespace Quoridor */
