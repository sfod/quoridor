#include "console.hpp"

#include <unistd.h>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include "player.hpp"
#include "exception.hpp"


namespace Quoridor {
namespace UI {

Console::Console(int player_num)
    : game_(std::shared_ptr<Board>(new Board(10, 10))), players_()
{
    static std::string colors[4] = {
        "red", "green", "pink", "blue"
    };

    if ((player_num != 2) && (player_num != 4)) {
        throw Exception("invalid number of players: "
                + boost::lexical_cast<std::string>(player_num));
    }


    for (int i = 0; i < player_num; ++i) {
        players_.push_back(std::shared_ptr<IPlayer>(new Player()));
        game_.add_pawn(std::shared_ptr<Pawn>(new Pawn(colors[i])));
    }
}

Console::~Console()
{
}

void Console::run()
{
    bool is_run = true;
    IMove *move;
    pos_t start_pos;
    pos_t end_pos;
    int rc;

    // main loop
    while (is_run) {
        for (size_t i = 0; i < players_.size(); ++i) {
            auto pawn = game_.pawn_list().at(i);
            auto player = players_[i];

            start_pos = game_.pawn_pos(pawn);

            while (true) {
                move = player->get_move();

                std::cout << "make move ";
                rc = game_.make_move(move, pawn);

                if (rc == 0) {
                    break;
                }
                // player should continue turn
                else if (rc == -2) {
                    continue;
                }
                // invalid move, player should make another move
                else if (rc == -1) {
                    continue;
                }
                // not possible
                else {
                    throw Exception("unknown return code from make_move :"
                            + boost::lexical_cast<std::string>(rc));
                }
            }

            end_pos = game_.pawn_pos(pawn);
            std::cout << pawn->color()
                << ": (" << start_pos.row << "," << start_pos.col <<
                ") => (" << end_pos.row << "," << end_pos.col << ")"
                << std::endl;

            if (game_.is_win(pawn)) {
                std::cout << pawn->color() << " win" << std::endl;
                is_run = false;
                break;
            }
        }

        usleep(500000);
    }
}

int Console::set_player(int i, std::shared_ptr<IPlayer> player)
{
    players_[i] = player;
    return 0;
}

}  /* namespace UI */
}  /* namespace Quoridor */
