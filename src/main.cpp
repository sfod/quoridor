#include "game/game_app.hpp"

int main(int argc, char **argv)
{
    try {
        GameApp app(argc, argv);
        return app.run();
    }
    catch (...) {
        return EXIT_FAILURE;
    }
}
