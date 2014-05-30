#ifndef QUORIDOR_PAWN_HPP_
#define QUORIDOR_PAWN_HPP_

#include <string>

namespace Quoridor {

class Pawn {
public:
    explicit Pawn(const std::string &color);
    virtual ~Pawn();

    const std::string& color() const { return color_; }

private:
    std::string color_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_PAWN_HPP_ */
