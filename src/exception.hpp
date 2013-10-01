#ifndef QUORIDOR_EXCEPTION_HPP_
#define QUORIDOR_EXCEPTION_HPP_

#include <stdexcept>

namespace Quoridor {

class Exception : public std::runtime_error {
public:
    Exception() : std::runtime_error("Quoridor Exception") {}
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_EXCEPTION_HPP_ */
