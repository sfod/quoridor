#include "console.hpp"

#include <unistd.h>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include "player.hpp"
#include "exception.hpp"
#include "walk_move.hpp"
#include "wall_move.hpp"


namespace Quoridor {
namespace UI {

Console::Console(int player_num)
    : game_(std::shared_ptr<Board>(new Board(9, 9))), players_(), repr_()
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

    repr_.resize(19);
    for (int i = 0; i < 19; ++i) {
        repr_[i].resize(19);
        for (int j = 0; j < 19; ++j) {
            if (i % 2 == 1) {
                repr_[i][j] = (j % 2 == 0 ? '|' : ' ');
            }
            else {
                repr_[i][j] = (j % 2 == 0 ? ' ' : '_');
            }
        }
    }

    for (int i = 0; i < player_num; ++i) {
        auto pawn = game_.pawn_list().at(i);
        pos_t pos = game_.pawn_pos(pawn);
        repr_[pos.row * 2 + 1][pos.col * 2 + 1] = pawn->color()[0];
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

    display();

    // main loop
    while (is_run) {
        for (size_t i = 0; i < players_.size(); ++i) {
            auto pawn = game_.pawn_list().at(i);
            auto player = players_[i];

            start_pos = game_.pawn_pos(pawn);

            while (true) {
                move = player->get_move();

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


            if (WalkMove *walk_move = dynamic_cast<WalkMove *>(move)) {
                std::cout << pawn->color() << std::endl;
                std::cout << "walk "
                    << walk_move->ToString((WalkMove::Direction)walk_move->dir());
                end_pos = game_.pawn_pos(pawn);
                std::cout << " (" << start_pos.row << ","
                    << start_pos.col << ") => (" << end_pos.row << ","
                    << end_pos.col << ")" << std::endl;
                repr_[start_pos.row * 2 + 1][start_pos.col * 2 + 1] = ' ';
                repr_[end_pos.row * 2 + 1][end_pos.col * 2 + 1] = pawn->color()[0];
            }
            else if (WallMove *wall_move = dynamic_cast<WallMove *>(move)) {
                const Wall &wall = wall_move->wall();
                std::cout << pawn->color() << std::endl;
                std::cout << "add wall: ("
                    << wall.orientation() << ", "
                    << wall.line() << ", "
                    << wall.start_pos() << ", "
                    << wall.cnt()
                    << ")" << std::endl;
                if (wall.orientation() == 0) {
                    for (int i = 0; i < wall.cnt(); ++i) {
                        repr_[wall.line() * 2 + 2][(wall.start_pos() + i) * 2 + 1] = '=';
                    }
                }
                else {
                    for (int i = 0; i < wall.cnt(); ++i) {
                        repr_[(wall.start_pos() + i) * 2 + 1][wall.line() * 2 + 2] = '$';
                    }
                }
            }

            display();

            if (game_.is_win(pawn)) {
                std::cout << pawn->color() << " win" << std::endl;
                is_run = false;
                break;
            }

            usleep(500000);
        }
    }
}

int Console::set_player(int i, std::shared_ptr<IPlayer> player)
{
    players_[i] = player;
    return 0;
}

void Console::display() const
{
    for (int i = 18; i >= 0; --i) {
        for (int j = 0; j < 19; ++j) {
            std::cout << repr_[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

}  /* namespace UI */
}  /* namespace Quoridor */
