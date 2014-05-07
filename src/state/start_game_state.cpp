#include "start_game_state.hpp"

// #include "game_state.hpp"
#include "main_menu_state.hpp"
#include "exception.hpp"

namespace Quoridor {

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
}

std::shared_ptr<CEGUI::Window> StartGameState::window() const
{
    return win_;
}

void StartGameState::subscribe_for_events_()
{
}

}  /* namespace Quoridor */
