#include <cstdlib>

#include <iostream>
#include <memory>

#include <boost/program_options.hpp>

#include "UI/ui_factory.hpp"
#include "player_factory.hpp"
#include "exception.hpp"

#include "state/main_menu_state.hpp"
#include "state/game_state.hpp"
#include "state/state_manager.hpp"


namespace po = boost::program_options;


struct game_opts_t {
    std::string ui_type;
};


int init(int argc, char **argv, game_opts_t *game_opts);

int main(int argc, char **argv)
{
    game_opts_t game_opts;

    if (init(argc, argv, &game_opts) < 0) {
        return EXIT_FAILURE;
    }

    Quoridor::StateManager stm;
    Quoridor::UI::UIFactory uif;

    stm.create_ui(uif, game_opts.ui_type);
    std::shared_ptr<Quoridor::IState> menu_state(new Quoridor::MainMenuState(stm.ui()));
    stm.change_state(std::shared_ptr<Quoridor::IState>(menu_state));

    stm.draw();
    while (stm.is_running()) {
        stm.handle_events();
        stm.update();
        stm.draw();
    }

    return EXIT_SUCCESS;
}

int init(int argc, char **argv, game_opts_t *game_opts)
{
    po::options_description options("Options");
    options.add_options()
        ("ui,i",
            po::value<std::string>(&game_opts->ui_type)->default_value("ncurses"),
            "user interface")
        ("help,h", "show help message")
    ;
    po::variables_map vm;

    try {
        po::store(po::parse_command_line(argc, argv, options), vm);
    }
    catch (po::error &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    po::notify(vm);

    if (vm.count("help")) {
        std::cout << options << std::endl;
        return -1;
    }

    return 0;
}
