#include "wall_grid.hpp"
#include "gtest/gtest.h"

using namespace Quoridor;

class WallGridTest : public ::testing::Test {
protected:
    WallGridTest() : wg_(10) {
    }

    WallGrid wg_;
};

TEST_F(WallGridTest, add_horizontal_walls)
{
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 1, 4, 2)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 1, 2, 2)));
}

TEST_F(WallGridTest, add_vertical_walls)
{
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kVertical, 1, 4, 2)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kVertical, 1, 2, 2)));
}

TEST_F(WallGridTest, add_horizontal_walls_starting_on_border)
{
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 1, 0, 2)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 3, 0, 1)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 8, 0, 3)));
}

TEST_F(WallGridTest, add_vertical_walls_starting_on_border)
{
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kVertical, 0, 1, 1)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kVertical, 0, 5, 4)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kVertical, 0, 8, 7)));
}

TEST_F(WallGridTest, add_horizontal_walls_ending_on_border)
{
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 1, 7, 2)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 5, 8, 1)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 8, 5, 4)));
}

TEST_F(WallGridTest, add_vertical_walls_ending_on_border)
{
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kVertical, 6, 1, 3)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kVertical, 7, 2, 2)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kVertical, 8, 8, 1)));
}

TEST_F(WallGridTest, add_adjacent_walls)
{
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 5, 4, 2)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kVertical, 4, 6, 2)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kVertical, 4, 4, 2)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 6, 3, 2)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 4, 5, 2)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 5, 2, 2)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 5, 6, 2)));
}

TEST_F(WallGridTest, add_walls_on_border)
{
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 0, 4, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 9, 0, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 8, 0, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 4, 9, 2)));
}

TEST_F(WallGridTest, add_walls_intersecting_border)
{
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 5, 8, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 1, 7, 5)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 5, -1, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 1, -3, 5)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 8, 8, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 6, 2, 5)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, -1, 8, 3)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, -2, 2, 3)));
}

TEST_F(WallGridTest, add_walls_beyond_board)
{
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 1, 9, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 10, 4, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 10, 10, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 7, 11, 1)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, -2, 4, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 1, -2, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, -1, -2, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 4, -4, 2)));

    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 9, 1, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 10, 6, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 10, 11, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, -3, 1, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 2, -9, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, -4, -2, 2)));
}

TEST_F(WallGridTest, add_intersecting_walls)
{
    // prepare walls
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 2, 0, 3)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 3, 1, 3)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kVertical, 4, 6, 3)));

    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 1, 2, 4)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 1, 3, 3)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 5, 5, 3)));
}

TEST_F(WallGridTest, add_overlapping_walls)
{
    // prepare walls
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kHorizontal, 2, 3, 3)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 2, 3, 3)));
    EXPECT_EQ(0, wg_.add_wall(Wall(Wall::kVertical, 4, 6, 3)));

    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 2, 3, 1)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 2, 2, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kHorizontal, 2, 5, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 4, 6, 2)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 2, 6, 4)));
    EXPECT_EQ(-1, wg_.add_wall(Wall(Wall::kVertical, 6, 6, 3)));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
