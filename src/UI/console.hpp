#ifndef QUORIDOR_UI_CONSOLE_HPP_
#define QUORIDOR_UI_CONSOLE_HPP_

#include "iplayer.hpp"
#include "irunnable.hpp"
#include "game.hpp"

namespace Quoridor {
namespace UI {

class Console : public IRunnable {
public:
    explicit Console(int player_num);
    virtual ~Console();

    void run();
    int set_player(int i, int type);

private:
    Game game_;
    std::vector<IPlayer *> players_;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_CONSOLE_HPP_ */
