#ifndef QUORIDOR_IRUNNABLE_HPP_
#define QUORIDOR_IRUNNABLE_HPP_

namespace Quoridor {

class IRunnable {
public:
    virtual ~IRunnable() {}
    virtual void run() = 0;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_IRUNNABLE_HPP_ */
