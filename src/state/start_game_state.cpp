#include "start_game_state.hpp"

#include "game_state.hpp"
#include "main_menu_state.hpp"
#include "logger.hpp"
#include "exception.hpp"


static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

namespace Quoridor {

std::string StartGameState::name_("Start Game");

StartGameState::StartGameState(std::shared_ptr<StateManager> stm)
    : stm_(stm), player_types_(), player_num_(0)
{
    win_ = std::shared_ptr<CEGUI::Window>(
            CEGUI::WindowManager::getSingleton().
                    loadLayoutFromFile("start_game.layout"),
            [=](CEGUI::Window *w) {
                BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "removing window " << w;
                CEGUI::WindowManager::getSingleton().destroyWindow(w);
            }
    );

    init_player_num_();

    subscribe_for_events_();
}

StartGameState::~StartGameState()
{
    BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "destroying state " << name_;
}

std::shared_ptr<CEGUI::Window> StartGameState::window() const
{
    return win_;
}

const std::string &StartGameState::name() const
{
    return name_;
}

void StartGameState::init_player_num_()
{
    CEGUI::Combobox *cbpn = static_cast<CEGUI::Combobox*>(win_->getChild("playerNum"));

    std::vector<std::pair<int, std::string>> num_str_list = {
        {2, "two players"},
        {4, "four players"}
    };
    for (auto num : num_str_list) {
        auto item = new CEGUI::ListboxTextItem(num.second);
        item->setUserData(reinterpret_cast<void*>(num.first));
        cbpn->addItem(item);
    }

    if (auto item = cbpn->getListboxItemFromIndex(0)) {
        cbpn->setItemSelectState(item, true);
        update_player_num_();
    }
    else {
        throw Exception("failed to set player number");
    }
}

int StartGameState::update_player_num_()
{
    CEGUI::Combobox *cbpn = static_cast<CEGUI::Combobox*>(win_->getChild("playerNum"));
    if (auto item = cbpn->getSelectedItem()) {
        player_num_ = reinterpret_cast<uintptr_t>(item->getUserData());
        BOOST_LOG_SEV(lg, boost::log::trivial::info)
            << "set player number to " << player_num_;
        set_player_list_();
        return 0;
    }
    return -1;
}

void StartGameState::set_player_list_()
{
    CEGUI::DefaultWindow *plist_win = static_cast<CEGUI::DefaultWindow*>(
            win_->getChild("playerList"));
    for (size_t i = 0; i < player_num_; ++i) {
        auto ptype_win = CEGUI::WindowManager::getSingleton().createWindow("GlossySerpent/Combobox");
        ptype_win->setPosition(CEGUI::UVector2(
                    CEGUI::UDim(0, 20),
                    CEGUI::UDim(0.1 * i, 0)));
        ptype_win->setSize(CEGUI::USize(
                    CEGUI::UDim(0.5, 20),
                    CEGUI::UDim(0.8, 0)));
        plist_win->addChild(ptype_win);
    }
}

void StartGameState::subscribe_for_events_()
{
    win_->getChild("startGame")->subscribeEvent(
            CEGUI::Window::EventMouseClick,
            CEGUI::Event::Subscriber(
                    &StartGameState::handle_start_game_, this
            )
    );
    win_->getChild("returnToMainMenu")->subscribeEvent(
            CEGUI::Window::EventMouseClick,
            CEGUI::Event::Subscriber(
                    &StartGameState::handle_return_, this
            )
    );
    win_->getChild("playerNum")->subscribeEvent(
            CEGUI::Combobox::EventListSelectionAccepted,
            CEGUI::Event::Subscriber(
                    &StartGameState::handle_player_num_, this
            )
    );
}

bool StartGameState::handle_start_game_(const CEGUI::EventArgs &/* e */)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "starting game";
    stm_->change_state(std::shared_ptr<IState>(new GameState(stm_, player_types_)));
    return true;
}

bool StartGameState::handle_return_(const CEGUI::EventArgs &/* e */)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "returning to main menu";
    stm_->change_state(std::shared_ptr<IState>(new MainMenuState(stm_)));
    return true;
}

bool StartGameState::handle_player_num_(const CEGUI::EventArgs &/* e */)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "changing player number";
    if (update_player_num_() == 0) {
        return true;
    }
    return false;
}

}  /* namespace Quoridor */
