#include <stdexcept>

namespace Quoridor {

class Exception : public std::runtime_error {
public:
    Exception() : std::runtime_error("Quoridor Exception") {}
};

}  /* namespace Quoridor */
