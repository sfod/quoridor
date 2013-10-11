#include "console.hpp"
#include <unistd.h>
#include <iostream>
#include "exception.hpp"

namespace Quoridor {
namespace UI {

Console::Console(int player_num)
    : game_(std::shared_ptr<Board>(new Board(10, 10)))
{
    static std::string colors[4] = {
        "red", "green", "pink", "blue"
    };
    if ((player_num != 2) && (player_num != 4)) {
        throw Exception();
    }
    for (int i = 0; i < player_num; ++i) {
        game_.add_pawn(std::shared_ptr<Pawn>(new Pawn(colors[i])));
    }
}

Console::~Console()
{
}

void Console::run()
{
    bool is_run = true;
    BoardMoves move;
    pos_t start_pos;
    pos_t end_pos;
    int rc;

    // main loop
    while (is_run) {
        for (auto pawn: game_.pawn_list()) {
            start_pos = game_.pawn_pos(pawn);

            while (true) {
                while (read_move(&move) < 0) {
                }

                std::cout << "make move ";
                while ((rc = game_.make_move(move, pawn)) == -1) {
                }

                // player have to continue turn
                if (rc == 0) {
                    break;
                }
                else if (rc == -2) {
                    continue;
                }
                // not possible
                else {
                    throw Exception();
                }
            }

            start_pos = game_.pawn_pos(pawn);
            std::cout << pawn->color()
                << ": (" << start_pos.first << "," << start_pos.second <<
                ") => (" << end_pos.first << "," << end_pos.second << ")"
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

int Console::read_move(BoardMoves *move)
{
    int m;
    // std::cin >> m;
    m = 0;
    if (m < kEND) {
        *move = static_cast<BoardMoves>(m);
    }
    return 0;
}

}  /* namespace UI */
}  /* namespace Quoridor */
