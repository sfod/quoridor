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
    int set_player(int i, std::shared_ptr<IPlayer> player);
    void display() const;

private:
    Game game_;
    std::vector<std::shared_ptr<IPlayer>> players_;
    std::vector<std::vector<char>> repr_;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_CONSOLE_HPP_ */
