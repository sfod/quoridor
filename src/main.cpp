#include <cstdlib>

#include <iostream>
#include <memory>

#include <boost/program_options.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>

#include "UI/ui_factory.hpp"
#include "player_factory.hpp"
#include "exception.hpp"

#include "state/main_menu_state.hpp"
#include "state/game_state.hpp"
#include "state/state_manager.hpp"


namespace po = boost::program_options;
namespace logging = boost::log;


struct game_opts_t {
    std::string ui_type;
};


BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, boost::log::sources::logger_mt)

int init(int argc, char **argv, game_opts_t *game_opts);

int main(int argc, char **argv)
{
    game_opts_t game_opts;

    if (init(argc, argv, &game_opts) < 0) {
        return EXIT_FAILURE;
    }

    // logging::sources::severity_logger<logging::trivial::severity_level> lg;
    boost::log::sources::logger_mt &lg = my_logger::get();
    BOOST_LOG_SEV(lg, logging::trivial::info) << "Creating " << game_opts.ui_type << " UI";

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
    std::string logfile;

    po::options_description options("Options");
    options.add_options()
    (
        "ui,i",
        po::value<std::string>(&game_opts->ui_type)->default_value("ncurses"),
        "user interface"
    )
    (
         "log,l",
        po::value<std::string>(&logfile)->default_value("quoridor.log"),
        "logging file"
    )
    (
         "help,h",
         "show help message"
    );
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

    logging::add_file_log(
        boost::log::keywords::file_name = logfile,
        boost::log::keywords::format = "[%TimeStamp%]: %Message%"
    );
    logging::add_common_attributes();

    return 0;
}
