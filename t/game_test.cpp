#include <fstream>
#include <memory>
#include <vector>

#include "game.hpp"
#include "pawn.hpp"
#include "logger.hpp"
#include "exception.hpp"

#include "mock_player.hpp"

using namespace Quoridor;
namespace logging = boost::log;

BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope", boost::log::attributes::named_scope::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", boost::log::trivial::severity_level)


static std::vector<std::string> colors = {"red", "green"};

static void init_logging(const std::string &logfile);

int main()
{
    init_logging("test_game.log");
    boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
    lg.add_attribute("Tag", blattrs::constant<std::string>("main"));

    std::vector<std::shared_ptr<Pawn>> pawn_list;
    std::shared_ptr<Pawn> pawn1(new Pawn(colors[0]));
    pawn_list.push_back(pawn1);
    std::shared_ptr<Pawn> pawn2(new Pawn(colors[1]));
    pawn_list.push_back(pawn2);

    std::shared_ptr<Game> game(new Game(9));
    try {
        game->set_pawns(pawn_list);
        game->switch_pawn();
    }
    catch (Exception &e) {
        BOOST_LOG_ERROR(lg) << "failed to create game: " << e.what();
        throw;
    }

    std::map<std::shared_ptr<Pawn>, std::shared_ptr<MockPlayer>> players;
    for (int i = 0; i < 2; ++i) {
        BOOST_LOG_INFO(lg) << "adding mock player";
        players[pawn_list[i]] = std::shared_ptr<MockPlayer>(new MockPlayer(game, pawn_list[i]));
    }

    players[pawn_list[1]]->push_move(new WalkMove(Node(7, 4)));
    players[pawn_list[1]]->push_move(new WalkMove(Node(6, 4)));
    players[pawn_list[1]]->push_move(new WallMove(Wall(Wall::kHorizontal, 8, 4, 2)));
    players[pawn_list[1]]->push_move(new WalkMove(Node(7, 4)));
    players[pawn_list[1]]->push_move(new WalkMove(Node(8, 4)));

    game->switch_pawn();
    for (int i = 0; i < 5; ++i) {
        IMove *move = players[pawn_list[1]]->get_move();
        if (move != NULL) {
            Node cur_node = game->cur_pawn_data().node;
            int rc;

            auto pawn = game->cur_pawn_data().pawn;
            if (WalkMove *walk_move = dynamic_cast<WalkMove*>(move)) {
                rc = game->move_pawn(walk_move->node());
                if (rc == 0) {
                    BOOST_LOG_DEBUG(lg) << pawn->color()
                        << " move: " << cur_node.row() << ":" << cur_node.col()
                        << " -> " << walk_move->node().row() << ":"
                        << walk_move->node().col();
                }
                else {
                    continue;
                }
            }
            else if (WallMove *wall_move = dynamic_cast<WallMove*>(move)) {
                rc = game->add_wall(wall_move->wall());
                if (rc == 0) {
                    BOOST_LOG_DEBUG(lg) << pawn->color() << " added "
                        << wall_move->wall();
                }
                else {
                    continue;
                }
            }
        }
    }

    // int idx = 0;
    // while (!game->is_finished()) {
    //     IMove *move = players[pawn_list[idx]]->get_move();
    //     if (move != NULL) {
    //         Node cur_node = game->cur_pawn_data().node;
    //         int rc;
    //
    //         auto pawn = game->cur_pawn_data().pawn;
    //         if (WalkMove *walk_move = dynamic_cast<WalkMove*>(move)) {
    //
    //             rc = game->move_pawn(walk_move->node());
    //             if (rc == 0) {
    //                 BOOST_LOG_DEBUG(lg) << pawn->color()
    //                     << " move: " << cur_node.row() << ":" << cur_node.col()
    //                     << " -> " << walk_move->node().row() << ":"
    //                     << walk_move->node().col();
    //             }
    //             else {
    //                 continue;
    //             }
    //         }
    //         else if (WallMove *wall_move = dynamic_cast<WallMove*>(move)) {
    //             rc = game->add_wall(wall_move->wall());
    //             if (rc == 0) {
    //                 BOOST_LOG_DEBUG(lg) << pawn->color() << " added "
    //                     << wall_move->wall();
    //             }
    //             else {
    //                 continue;
    //             }
    //         }
    //         idx = 1 - idx;
    //         game->switch_pawn();
    //     }
    // }

    // idx = 1 - idx;
    // BOOST_LOG_INFO(lg) << game->cur_pawn_data().pawn->color() << " wins";

    return 0;
}

void formatter(boost::log::record_view const &rec, boost::log::formatting_ostream &strm)
{
    strm << rec[timestamp]
        << " [" << rec[severity] << "] ";

    boost::log::attributes::named_scope_list scope_list = rec[scope].get();
    for (auto iter = scope_list.begin(); iter != scope_list.end(); ++iter) {
        strm << "(" << iter->scope_name << ") ";
    }

    strm << "(" << rec[tag_attr] << ") ";
    strm << rec[boost::log::expressions::smessage];
}

static void init_logging(const std::string &logfile)
{
    typedef boost::log::sinks::synchronous_sink<
            boost::log::sinks::text_ostream_backend> text_sink;
    boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();

    sink->locked_backend()->add_stream(
            boost::make_shared<std::ofstream>(logfile));
    sink->locked_backend()->auto_flush(true);
    sink->set_formatter(&formatter);

    boost::log::core::get()->add_sink(sink);

    boost::log::add_console_log(
        std::clog,
        boost::log::keywords::format = (
            boost::log::expressions::stream
                << "[" << severity << "] (" << tag_attr << ") "
                << boost::log::expressions::smessage
        )
    );

    boost::log::add_common_attributes();
    boost::log::core::get()->add_global_attribute("Scope",
            boost::log::attributes::named_scope());
}
