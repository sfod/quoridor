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
    if ((player_num != 2) || (player_num != 4)) {
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
    while (is_run) {
        for (auto pawn: game_.pawn_list()) {
            while (true) {
                while (read_move(&move) < 0) {
                }
                while (game_.make_move(pawn) < 0) {
                }
            }
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
    std::cin >> m;
    if (m < kEND) {
        *move = static_cast<BoardMoves>(m);
    }
    return 0;
}

}  /* namespace UI */
}  /* namespace Quoridor */
