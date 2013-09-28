#include <cstdlib>

#include <iostream>
#include <memory>

#include "board.hpp"
#include "game.hpp"
#include "player.hpp"

int main()
{
    Quoridor::Game game(std::shared_ptr<Quoridor::Board>(new Quoridor::Board(9, 9)));
    game.add_player(std::shared_ptr<Quoridor::Player>(new Quoridor::Player("white")));
    game.add_player(std::shared_ptr<Quoridor::Player>(new Quoridor::Player("black")));

    game.main_loop();

    return EXIT_SUCCESS;
}
