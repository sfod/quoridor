#include "board.hpp"
#include "exception.hpp"
#include "gtest/gtest.h"

class BoardTest : public ::testing::Test {
protected:
    BoardTest() : board_(9) {
    }

    Quoridor::Board board_;
};

TEST_F(BoardTest, ctor)
{
    // test invalid Board construction
    EXPECT_THROW(Quoridor::Board(-1), Quoridor::Exception);
    EXPECT_THROW(Quoridor::Board(0), Quoridor::Exception);
    EXPECT_THROW(Quoridor::Board(2), Quoridor::Exception);
}

TEST_F(BoardTest, add_wall)
{
    // add valid walls
    EXPECT_EQ(0, board_.add_wall(Quoridor::Wall(0, 0, 0, 2)));
    EXPECT_EQ(0, board_.add_wall(Quoridor::Wall(0, 0, 2, 2)));

    // add walls beyond the board
    EXPECT_EQ(-1, board_.add_wall(Quoridor::Wall(0, 10, 0, 2)));

    // add intersecting walls
    EXPECT_EQ(-1, board_.add_wall(Quoridor::Wall(0, 0, 0, 1)));
}

TEST_F(BoardTest, block_path)
{
    std::shared_ptr<Quoridor::Pawn> pawn(new Quoridor::Pawn("black"));
    board_.add_pawn(pawn);
    EXPECT_EQ(0, board_.add_wall(Quoridor::Wall(0, 2, 0, 2)));
    EXPECT_EQ(0, board_.add_wall(Quoridor::Wall(0, 2, 2, 2)));
    EXPECT_EQ(0, board_.add_wall(Quoridor::Wall(0, 2, 4, 2)));
    EXPECT_EQ(0, board_.add_wall(Quoridor::Wall(0, 2, 6, 2)));
    EXPECT_EQ(0, board_.add_wall(Quoridor::Wall(0, 2, 8, 1)));
    EXPECT_EQ(-1, board_.add_wall(Quoridor::Wall(0, 2, 9, 2)));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
