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

#include "player_factory.hpp"
#include "exception.hpp"

#include "state/state_manager.hpp"
#include "state/main_menu_state.hpp"


namespace po = boost::program_options;
namespace logging = boost::log;


BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, boost::log::sources::logger)

int init(int argc, char **argv);

int main(int argc, char **argv)
{
    if (init(argc, argv) < 0) {
        return EXIT_FAILURE;
    }

    boost::log::sources::logger &lg = my_logger::get();
    BOOST_LOG_SEV(lg, logging::trivial::info) << "initializing game";

    std::shared_ptr<Quoridor::StateManager> stm(new Quoridor::StateManager);
    std::shared_ptr<Quoridor::IState> menu_state(new Quoridor::MainMenuState(stm));

    stm->change_state(std::shared_ptr<Quoridor::IState>(menu_state));
    stm->draw();
    while (stm->is_running()) {
        stm->handle_events();
        stm->draw();
    }

    return EXIT_SUCCESS;
}

int init(int argc, char **argv)
{
    std::string logfile;

    po::options_description options("Options");
    options.add_options()
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
