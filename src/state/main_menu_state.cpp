#include "main_menu_state.hpp"
#include "start_game_state.hpp"

namespace Quoridor {

static std::vector<std::string> items = {"game", "quit"};

MainMenuState::MainMenuState(std::shared_ptr<UI::UI> ui)
{
    win_ = ui->create_window();
    menu_ = ui->create_menu(items);
    win_->add_menu(menu_);
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::handle_events(StateManager *stm)
{
    std::shared_ptr<UI::UI> ui = stm->ui();
    UI::Event ev;

    if (ui->poll_event(&ev)) {
        switch (ev) {
        case UI::kUp:
            menu_->up();
            break;
        case UI::kDown:
            menu_->down();
            break;
        case UI::kEnter: {
            std::string menu_item = menu_->item();
            if (menu_item == "game") {
                std::shared_ptr<IState> game_state(new StartGameState(ui));
                stm->change_state(std::shared_ptr<IState>(game_state));
            }
            else if (menu_item == "quit") {
                stm->stop();
            }
            break;
        }
        case UI::kEsc:
            stm->stop();
            break;
        default:
            break;
        }
    }
}

void MainMenuState::update()
{
}

void MainMenuState::draw()
{
    win_->draw();
}

void MainMenuState::change_state()
{
}

}  /* namespace Quoridor */
