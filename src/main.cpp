#include <cstdlib>

#include <iostream>

#include "board.hpp"
#include "game.hpp"
#include "player.hpp"

int main()
{
    Quoridor::Board board(10, 10);
    Quoridor::Player player_white("white", &board);
    Quoridor::Player player_black("black", &board);

    Quoridor::Game game(board);
    game.add_player(player_white);
    game.add_player(player_black);

    game.main_loop();

    return EXIT_SUCCESS;
}
