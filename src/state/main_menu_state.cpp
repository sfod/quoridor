#include "main_menu_state.hpp"

#include <iostream>
#include "start_game_state.hpp"
#include "logger.hpp"


static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

namespace Quoridor {

MainMenuState::MainMenuState(std::shared_ptr<StateManager> stm)
    : stm_(stm),
    win_(CEGUI::WindowManager::getSingleton().loadLayoutFromFile("main_menu.layout"))
{
    subscribe_for_events_();
}

MainMenuState::~MainMenuState()
{
}

std::shared_ptr<CEGUI::Window> MainMenuState::window() const
{
    return win_;
}

void MainMenuState::subscribe_for_events_()
{
    win_->getChild("mainMenuWindow/newGame")->subscribeEvent(
            CEGUI::Window::EventMouseClick,
            CEGUI::Event::Subscriber(
                    &MainMenuState::handle_new_game_, this
            )
    );
    win_->getChild("mainMenuWindow/quitGame")->subscribeEvent(
            CEGUI::Window::EventMouseClick,
            CEGUI::Event::Subscriber(
                    &MainMenuState::handle_quit_game_, this
            )
    );
}

bool MainMenuState::handle_new_game_(const CEGUI::EventArgs &/* e */)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "starting new game";
    stm_->change_state(std::shared_ptr<IState>(new StartGameState(stm_)));
    return true;
}

bool MainMenuState::handle_quit_game_(const CEGUI::EventArgs &/* e */)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "quit game";
    stm_->stop();
    return true;
}

}  /* namespace Quoridor */
