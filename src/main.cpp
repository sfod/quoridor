#include <cstdlib>

#include <iostream>
#include <memory>

#include <boost/program_options.hpp>

#include "player_factory.hpp"
#include "UI/console.hpp"

namespace po = boost::program_options;


struct game_opts_t {
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

    Quoridor::PlayerFactory pf;
    Quoridor::UI::Console console(game_opts.player_num);

    for (size_t i = 0; i < game_opts.players.size(); ++i) {
        console.set_player(i, pf.make_player(game_opts.players[i]));
    }

    console.run();

    return EXIT_SUCCESS;
}

int init(int argc, char **argv, game_opts_t *game_opts)
{
    po::options_description options("Options");
    options.add_options()
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
