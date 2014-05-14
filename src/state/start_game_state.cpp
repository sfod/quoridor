#include "start_game_state.hpp"

#include "game_state.hpp"
#include "main_menu_state.hpp"
#include "logger.hpp"
#include "exception.hpp"


static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

namespace Quoridor {

std::string StartGameState::name_("Start Game");

StartGameState::StartGameState(std::shared_ptr<StateManager> stm)
    : stm_(stm), player_types_(), player_num_(2)
{
    win_ = std::shared_ptr<CEGUI::Window>(
            CEGUI::WindowManager::getSingleton().
                    loadLayoutFromFile("start_game.layout"),
            [=](CEGUI::Window *w) {
                BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "removing window " << w;
                CEGUI::WindowManager::getSingleton().destroyWindow(w);
            }
    );
    subscribe_for_events_();

    player_types_.push_back("fake");
    player_types_.push_back("fake");
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


}  /* namespace Quoridor */
