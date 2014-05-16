#include "main_menu_state.hpp"

#include <iostream>
#include "start_game_state.hpp"
#include "logger.hpp"


static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

namespace Quoridor {

std::string MainMenuState::name_("Main Menu");

MainMenuState::MainMenuState(std::shared_ptr<StateManager> stm) : stm_(stm)
{
    win_ = std::shared_ptr<CEGUI::Window>(
            CEGUI::WindowManager::getSingleton().
                    loadLayoutFromFile("main_menu.layout"),
            [=](CEGUI::Window *w) {
                BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "removing window " << w;
                CEGUI::WindowManager::getSingleton().destroyWindow(w);
            }
    );
    subscribe_for_events_();
}

MainMenuState::~MainMenuState()
{
    BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "destroying state " << name_;
}

void MainMenuState::update()
{
}

std::shared_ptr<CEGUI::Window> MainMenuState::window() const
{
    return win_;
}

const std::string &MainMenuState::name() const
{
    return name_;
}

void MainMenuState::subscribe_for_events_()
{
    win_->getChild("newGame")->subscribeEvent(
            CEGUI::Window::EventMouseClick,
            CEGUI::Event::Subscriber(
                    &MainMenuState::handle_new_game_, this
            )
    );
    win_->getChild("quitGame")->subscribeEvent(
            CEGUI::Window::EventMouseClick,
            CEGUI::Event::Subscriber(
                    &MainMenuState::handle_quit_game_, this
            )
    );
}

bool MainMenuState::handle_new_game_(const CEGUI::EventArgs &/* e */)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "creating new game";
    std::shared_ptr<IState> state;
    try {
        state = std::shared_ptr<IState>(new StartGameState(stm_));
        stm_->change_state(state);
    }
    catch (CEGUI::Exception &e) {
        BOOST_LOG_SEV(lg, boost::log::trivial::fatal)
            << "failed to create StartGameState";
        BOOST_LOG_SEV(lg, boost::log::trivial::fatal) << e.what();
        stm_->stop();
    }

    return true;
}

bool MainMenuState::handle_quit_game_(const CEGUI::EventArgs &/* e */)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "quit game";
    stm_->stop();
    return true;
}

}  /* namespace Quoridor */
