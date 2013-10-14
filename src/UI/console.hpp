#ifndef QUORIDOR_UI_CONSOLE_HPP_
#define QUORIDOR_UI_CONSOLE_HPP_

#include "irunnable.hpp"
#include "game.hpp"

namespace Quoridor {
namespace UI {

class Console : public IRunnable {
public:
    explicit Console(int player_num);
    virtual ~Console();

    void run();

private:
    int read_move(BoardMoves *move);

private:
    Game game_;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_CONSOLE_HPP_ */
