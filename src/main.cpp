#include <cstdlib>

#include <iostream>
#include <memory>

#include "board.hpp"
#include "game.hpp"
#include "pawn.hpp"

int main()
{
    Quoridor::Game game(std::shared_ptr<Quoridor::Board>(new Quoridor::Board(9, 9)));
    game.add_pawn(std::shared_ptr<Quoridor::Pawn>(new Quoridor::Pawn("white")));
    game.add_pawn(std::shared_ptr<Quoridor::Pawn>(new Quoridor::Pawn("black")));

    game.main_loop();

    return EXIT_SUCCESS;
}
