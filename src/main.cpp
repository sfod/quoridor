#include <cstdlib>

#include <fstream>
#include <memory>

#include <boost/program_options.hpp>

#include "UI/ui_factory.hpp"
#include "player_factory.hpp"
#include "exception.hpp"
#include "logger.hpp"

#include "state/main_menu_state.hpp"
#include "state/game_state.hpp"
#include "state/state_manager.hpp"


namespace po = boost::program_options;
namespace logging = boost::log;


struct game_opts_t {
    std::string ui_type;
};


BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope", boost::log::attributes::named_scope::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)



static int init(int argc, char **argv, game_opts_t *game_opts);
static void init_logging(const std::string &logfile);

int main(int argc, char **argv)
{
    game_opts_t game_opts;

    if (init(argc, argv, &game_opts) < 0) {
        return EXIT_FAILURE;
    }

    boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
    BOOST_LOG_SEV(lg, logging::trivial::info) << "Creating "
        << game_opts.ui_type << " UI";

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

static int init(int argc, char **argv, game_opts_t *game_opts)
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

    init_logging(logfile);

    return 0;
}

void formatter(boost::log::record_view const &rec, boost::log::formatting_ostream &strm)
{
    strm << rec[timestamp]
        << " [" << rec[boost::log::trivial::severity] << "] ";

    boost::log::attributes::named_scope_list scope_list = rec[scope].get();
    for (auto iter = scope_list.begin(); iter != scope_list.end(); ++iter) {
        strm << "(" << iter->scope_name << ") ";
    }

    strm << rec[boost::log::expressions::smessage];
}

static void init_logging(const std::string &logfile)
{
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> text_sink;
    boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();

    sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>(logfile, std::ios_base::app));
    sink->locked_backend()->auto_flush(true);
    sink->set_formatter(&formatter);

    boost::log::core::get()->add_sink(sink);

    boost::log::add_console_log(
        std::clog,
        boost::log::keywords::format = (
            boost::log::expressions::stream
                << "[" << boost::log::trivial::severity << "] "
                << boost::log::expressions::smessage
        )
    );

    boost::log::add_common_attributes();
    boost::log::core::get()->add_global_attribute("Scope",
            boost::log::attributes::named_scope());
}
