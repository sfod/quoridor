#include <cstdlib>

#include <iostream>

#include "board.hpp"
#include "game.hpp"
#include "player.hpp"

int main()
{
    Quoridor::Game game(new Quoridor::Board(9, 9));
    game.add_player(new Quoridor::Player("white"));
    game.add_player(new Quoridor::Player("black"));

    game.main_loop();

    return EXIT_SUCCESS;
}
