#ifndef QUORIDOR_EXCEPTION_HPP_
#define QUORIDOR_EXCEPTION_HPP_

#include <stdexcept>

namespace Quoridor {

class Exception : public std::runtime_error {
public:
    explicit Exception(const std::string &msg)
        : std::runtime_error(msg) {}
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_EXCEPTION_HPP_ */
