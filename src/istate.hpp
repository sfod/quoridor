#ifndef QUORIDOR_ISTATE_HPP_
#define QUORIDOR_ISTATE_HPP_

namespace Quoridor {

class IState {
public:
    IState() {}
    virtual ~IState() {}

    virtual void handle_events() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void change_state() = 0;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_ISTATE_HPP_ */
