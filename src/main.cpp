#include <cstdlib>

#include <iostream>
#include <memory>

#include <boost/program_options.hpp>

#include "UI/console.hpp"
#include "pawn.hpp"

namespace po = boost::program_options;

int main(int argc, char **argv)
{
    po::options_description options("Options");
    options.add_options()
        ("players,p", po::value<int>(), "number of players")
        ("help,h", "show help message")
        ;
    po::variables_map vm;

    try {
        po::store(po::parse_command_line(argc, argv, options), vm);
    }
    catch (po::error &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    po::notify(vm);

    if (vm.count("help")) {
        std::cout << options << std::endl;
        return EXIT_FAILURE;
    }

    int players = 2;
    if (vm.count("players")) {
        players = vm["players"].as<int>();
    }

    Quoridor::UI::Console console(players);
    console.run();

    return EXIT_SUCCESS;
}
