#include "start_game_state.hpp"

#include "game_state.hpp"
#include "main_menu_state.hpp"
#include "exception.hpp"

namespace Quoridor {

static std::vector<std::string> items = {"start", "quit"};

StartGameState::StartGameState(std::shared_ptr<UI::UI> ui)
    : win_(), menu_(), pf_(), players_(), player_num_(2)
{
    win_ = ui->create_window();
    menu_ = ui->create_menu(items);
    win_->add_menu(menu_);

    players_.push_back(pf_.make_player("fake"));
    players_.push_back(pf_.make_player("fake"));
}

StartGameState::~StartGameState()
{
}

void StartGameState::handle_events(StateManager *stm)
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
            if (menu_item == "start") {
                std::shared_ptr<IState> game_state(new GameState());
                stm->change_state(std::shared_ptr<IState>(game_state));
            }
            else if (menu_item == "quit") {
                std::shared_ptr<IState> menu_state(new MainMenuState(ui));
                stm->change_state(std::shared_ptr<IState>(menu_state));
            }
            break;
        }
        case UI::kEsc: {
            std::shared_ptr<Quoridor::IState> menu_state(new MainMenuState(ui));
            stm->change_state(std::shared_ptr<IState>(menu_state));
            break;
        }
        default:
            break;
        }
    }
}

void StartGameState::update()
{
}

void StartGameState::draw()
{
    win_->draw();
}

void StartGameState::change_state()
{
}

void StartGameState::set_player(int i, std::shared_ptr<IPlayer> player)
{
    if ((i < 0) || (i > 3)) {
        throw Exception("Invalid player number");
    }

    if ((i > 1) && (player_num_ == 2)) {
        player_num_ = 4;
        players_.push_back(pf_.make_player("fake"));
        players_.push_back(pf_.make_player("fake"));
    }

    players_[i] = player;
}

}  /* namespace Quoridor */
