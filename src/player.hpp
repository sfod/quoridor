#ifndef QUORIDOR_PLAYER_HPP_
#define QUORIDOR_PLAYER_HPP_

#include <string>

namespace Quoridor {

class Player {
public:
    explicit Player(const std::string &name);
    virtual ~Player();

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_PLAYER_HPP_ */
