#ifndef QUORIDOR_WALK_MOVE_HPP_
#define QUORIDOR_WALK_MOVE_HPP_

#include "imove.hpp"
#include <boost/preprocessor.hpp>

#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)    \
    case elem : return BOOST_PP_STRINGIZE(elem);

#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)                \
    enum name {                                                               \
        BOOST_PP_SEQ_ENUM(enumerators)                                        \
    };                                                                        \
                                                                              \
    inline const char* ToString(name v)                                       \
    {                                                                         \
        switch (v)                                                            \
        {                                                                     \
            BOOST_PP_SEQ_FOR_EACH(                                            \
                X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,          \
                name,                                                         \
                enumerators                                                   \
            )                                                                 \
            default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";         \
        }                                                                     \
    }

namespace Quoridor {

class WalkMove : public IMove {
public:
    WalkMove();
    explicit WalkMove(int dir);
    virtual ~WalkMove();

    int dir() const { return dir_; }
    void set_dir(int dir) { dir_ = dir; }

public:
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(Direction, (kForward)(kRight)(kBackward)(kLeft)(kEnd))

private:
    int dir_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_WALK_MOVE_HPP_ */
