#include "start_game_state.hpp"

// #include "game_state.hpp"
#include "main_menu_state.hpp"
#include "logger.hpp"
#include "exception.hpp"


static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

namespace Quoridor {

std::string StartGameState::name_("Start Game");

StartGameState::StartGameState(std::shared_ptr<StateManager> stm)
    : stm_(stm),
    win_(CEGUI::WindowManager::getSingleton().loadLayoutFromFile("start_game.layout")),
    player_types_(), player_num_(2)
{
    subscribe_for_events_();

    player_types_.push_back("fake");
    player_types_.push_back("fake");
}

StartGameState::~StartGameState()
{
    BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "destroying state " << name_;
    CEGUI::WindowManager::getSingleton().destroyWindow(win_.get());
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
}

}  /* namespace Quoridor */
