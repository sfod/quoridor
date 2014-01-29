#include <cstdlib>

#include <iostream>
#include <memory>

#include <boost/program_options.hpp>

#include "player_factory.hpp"
#include "UI/runner.hpp"
#include "exception.hpp"

namespace po = boost::program_options;


struct game_opts_t {
    std::string ui_type;
    int player_num;
    std::vector<int> players;
};


int init(int argc, char **argv, game_opts_t *game_opts);


int main(int argc, char **argv)
{
    game_opts_t game_opts;

    if (init(argc, argv, &game_opts) < 0) {
        return EXIT_FAILURE;
    }

    Quoridor::UI::Runner runner;
    Quoridor::UI::UIFactory uif;
    Quoridor::PlayerFactory pf;

    runner.create_ui(uif, game_opts.ui_type);

    for (int i = 0; i < game_opts.player_num; ++i) {
        runner.set_player(i, pf.make_player(game_opts.players[i]));
    }

    try {
        runner.run();
    }
    catch (Quoridor::Exception &e) {
        std::cerr << "failed to run game: " << e.what() << std::endl;
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
        ("player-num,n",
            po::value<int>(&game_opts->player_num)->default_value(2),
            "number of players")
        ("players,p", po::value<std::vector<int>>(&game_opts->players),
            "type of players")
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
