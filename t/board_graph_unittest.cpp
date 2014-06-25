#include "board_graph.hpp"
#include "gtest/gtest.h"

using namespace Quoridor;

class BoardGraphTest : public ::testing::Test {
protected:
    BoardGraphTest() : bg_(5, 5) {
    }

    BoardGraph bg_;
};

TEST_F(BoardGraphTest, find_path_on_empty_board_same_row)
{
    std::list<Node> path;
    bg_.find_path(Node(0, 1), Node(0, 3), &path);
    std::list<Node> expected_path = {
        Node(0, 2),
        Node(0, 3)
    };
    EXPECT_EQ(expected_path, path);
}

TEST_F(BoardGraphTest, find_path_on_empty_board_same_col)
{
    std::list<Node> path;
    bg_.find_path(Node(0, 1), Node(4, 1), &path);
    std::list<Node> expected_path = {
        Node(1, 1),
        Node(2, 1),
        Node(3, 1),
        Node(4, 1)
    };
    EXPECT_EQ(expected_path, path);
}

// for this test there are multiple paths with minimal length, so it will be
// right to check found path length and it's last element only
TEST_F(BoardGraphTest, find_path_on_empty_board_different_row_and_col)
{
    std::list<Node> path;
    bg_.find_path(Node(1, 1), Node(4, 3), &path);
    EXPECT_EQ(5, path.size());
    EXPECT_EQ(Node(4, 3), path.back());
}

TEST_F(BoardGraphTest, find_path_on_board_with_walls_same_row)
{
    bg_.remove_edges(Node(4,2), Node(4,3));

    std::list<Node> path;
    bg_.find_path(Node(4, 2), Node(4, 3), &path);
    std::list<Node> expected_path = {
        Node(3, 2),
        Node(3, 3),
        Node(4, 3)
    };
    EXPECT_EQ(expected_path, path);
}

// for this test there are multiple paths with minimal length, so it will be
// right to check found path length and it's last element only
TEST_F(BoardGraphTest, find_path_on_board_with_walls_same_col)
{
    bg_.remove_edges(Node(1,2), Node(2,2));

    std::list<Node> path;
    bg_.find_path(Node(0, 2), Node(4, 2), &path);
    EXPECT_EQ(6, path.size());
    EXPECT_EQ(Node(4, 2), path.back());
}

TEST_F(BoardGraphTest, block_node_wo_surroundings)
{
    bg_.block_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_TRUE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, unblock_node_wo_surroundings)
{
    bg_.block_node(Node(2, 2));
    bg_.unblock_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, block_node_near_blocked_node)
{
    bg_.block_node(Node(2, 3));
    bg_.block_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_TRUE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, unblock_node_near_blocked_node)
{
    bg_.block_node(Node(2, 3));
    bg_.block_node(Node(2, 2));
    bg_.unblock_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, block_node_near_blocked_node_2)
{
    bg_.block_node(Node(2, 2));
    bg_.block_node(Node(2, 3));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_TRUE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, unblock_node_near_blocked_node_2)
{
    bg_.block_node(Node(2, 2));
    bg_.block_node(Node(2, 3));
    bg_.unblock_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, block_node_adjacent_to_border)
{
    bg_.block_node(Node(1, 0));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 0), Node(0, 0)));
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 0), Node(2, 0)));
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 0), Node(1, 1)));

    // edges from neighbour nodes to the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(0, 0), Node(1, 0)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 0), Node(1, 0)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 1), Node(1, 0)));

    // nodes across the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(0, 0), Node(2, 0)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 0), Node(0, 0)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(0, 0), Node(1, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 0), Node(1, 1)));
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 1), Node(0, 0)));
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 1), Node(2, 0)));
}

TEST_F(BoardGraphTest, unblock_node_adjacent_to_border)
{
    bg_.block_node(Node(1, 0));
    bg_.unblock_node(Node(1, 0));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 0), Node(0, 0)));
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 0), Node(2, 0)));
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 0), Node(1, 1)));

    // edges from neighbour nodes to the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(0, 0), Node(1, 0)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 0), Node(1, 0)));
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 1), Node(1, 0)));

    // nodes across the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(0, 0), Node(2, 0)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 0), Node(0, 0)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(0, 0), Node(1, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 0), Node(1, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 1), Node(0, 0)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 1), Node(2, 0)));
}

TEST_F(BoardGraphTest, block_node_in_the_board_corner)
{
    bg_.block_node(Node(4, 4));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(4, 4), Node(4, 3)));
    EXPECT_TRUE(bg_.is_adjacent(Node(4, 4), Node(3, 4)));

    // edges from neighbour nodes to the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(4, 3), Node(4, 4)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 4), Node(4, 4)));

    // diagonal nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(4, 3), Node(3, 4)));
    EXPECT_TRUE(bg_.is_adjacent(Node(3, 4), Node(4, 3)));
}

TEST_F(BoardGraphTest, unblock_node_in_the_board_corner)
{
    bg_.block_node(Node(4, 4));
    bg_.unblock_node(Node(4, 4));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(4, 4), Node(4, 3)));
    EXPECT_TRUE(bg_.is_adjacent(Node(4, 4), Node(3, 4)));

    // edges from neighbour nodes to the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(4, 3), Node(4, 4)));
    EXPECT_TRUE(bg_.is_adjacent(Node(3, 4), Node(4, 4)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(4, 3), Node(3, 4)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 4), Node(4, 3)));
}

TEST_F(BoardGraphTest, block_node_adjacent_to_wall)
{
    bg_.remove_edges(Node(2, 2), Node(3, 2));
    bg_.block_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, unblock_node_adjacent_to_wall)
{
    bg_.remove_edges(Node(2, 2), Node(3, 2));
    bg_.block_node(Node(2, 2));
    bg_.unblock_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, block_node_in_walls_corner)
{
    bg_.remove_edges(Node(2, 2), Node(3, 2));
    bg_.remove_edges(Node(2, 2), Node(2, 3));
    bg_.block_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, unblock_node_in_walls_corner)
{
    bg_.remove_edges(Node(2, 2), Node(3, 2));
    bg_.remove_edges(Node(2, 2), Node(2, 3));
    bg_.block_node(Node(2, 2));
    bg_.unblock_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, block_node_between_walls)
{
    bg_.remove_edges(Node(2, 2), Node(1, 2));
    bg_.remove_edges(Node(2, 2), Node(3, 2));
    bg_.block_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, unblock_node_between_walls)
{
    bg_.remove_edges(Node(2, 2), Node(1, 2));
    bg_.remove_edges(Node(2, 2), Node(3, 2));
    bg_.block_node(Node(2, 2));
    bg_.unblock_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, block_node_diagonal_to_wall)
{
    bg_.remove_edges(Node(2, 3), Node(3, 3));
    bg_.block_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_TRUE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

TEST_F(BoardGraphTest, unblock_node_diagonal_to_wall)
{
    bg_.remove_edges(Node(2, 3), Node(3, 3));
    bg_.block_node(Node(2, 2));
    bg_.unblock_node(Node(2, 2));

    // edges from the blocked node to neighbour nodes
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(1, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 1)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(3, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 2), Node(2, 3)));

    // edges from neighbour nodes to the blocked node
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 1), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(3, 2), Node(2, 2)));
    EXPECT_TRUE(bg_.is_adjacent(Node(2, 3), Node(2, 2)));

    // nodes across the blocked node
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(2, 1)));

    // diagonal nodes
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(1, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 1), Node(1, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 1)));
    EXPECT_FALSE(bg_.is_adjacent(Node(3, 2), Node(2, 3)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(3, 2)));
    EXPECT_FALSE(bg_.is_adjacent(Node(2, 3), Node(1, 2)));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
