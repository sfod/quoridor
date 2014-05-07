#include "main_menu_state.hpp"
#include <iostream>
// #include "start_game_state.hpp"

namespace Quoridor {

static std::vector<std::string> items = {"game", "quit"};

MainMenuState::MainMenuState()
{
    win_ = std::shared_ptr<CEGUI::Window>(CEGUI::WindowManager::getSingleton().loadLayoutFromFile("main_menu.layout"));
    subscribe_for_events_();
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::handle_events(StateManager * /* stm */)
{
}

void MainMenuState::update()
{
}

void MainMenuState::draw()
{
}

void MainMenuState::change_state()
{
}

std::shared_ptr<CEGUI::Window> MainMenuState::window() const
{
    return win_;
}

void MainMenuState::subscribe_for_events_()
{
    win_->getChild("mainMenuWindow/startGame")->subscribeEvent(
            CEGUI::Window::EventMouseClick,
            CEGUI::Event::Subscriber(
                    &MainMenuState::handle_start_game_, this
            )
    );
}

bool MainMenuState::handle_start_game_(const CEGUI::EventArgs &/* e */)
{
    std::cout << "starting game..." << std::endl;
    return true;
}

}  /* namespace Quoridor */
