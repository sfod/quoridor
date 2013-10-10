#ifndef QUORIDOR_UI_CONSOLE_HPP_
#define QUORIDOR_UI_CONSOLE_HPP_

#include "game.hpp"

namespace Quoridor {
namespace UI {

class Console {
public:
    explicit Console(int player_num);
    virtual ~Console();

    void run();

private:
    Game game_;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_CONSOLE_HPP_ */
