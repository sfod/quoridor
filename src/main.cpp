#include <cstdlib>

#include <iostream>
#include <memory>

#include "UI/console.hpp"
#include "pawn.hpp"

int main()
{
    Quoridor::UI::Console console(2);
    console.run();

    return EXIT_SUCCESS;
}
