#ifndef QUORIDOR_BOARD_GRAPH_HPP_
#define QUORIDOR_BOARD_GRAPH_HPP_

#include <vector>

#include <boost/graph/astar_search.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/filtered_graph.hpp>

#include "node.hpp"


namespace Quoridor {

typedef boost::adjacency_list<
        boost::listS,
        boost::vecS,
        boost::directedS,
        boost::no_property,
        boost::property<boost::edge_weight_t, int>
> graph_t;
typedef boost::property_map<graph_t, boost::edge_weight_t>::type WeightMap;
typedef graph_t::vertex_descriptor vertex_descriptor;
typedef graph_t::edge_descriptor edge_descriptor;

typedef boost::property_map<graph_t, boost::vertex_index_t>::type IndexMap;
typedef boost::graph_traits<graph_t>::adjacency_iterator adjacency_iterator;

class FilterEdges {
public:
    FilterEdges();
    ~FilterEdges();

    void add_edge(const edge_descriptor &ed);
    void clear();
    template <typename EdgeDesc>
    bool operator()(const EdgeDesc &e) const;

private:
    std::set<edge_descriptor> edges_;
};

struct found_goal {}; // exception for termination

// visitor that terminates when we find the goal
template <class Vertex>
class astar_goal_visitor : public boost::default_astar_visitor
{
public:
    astar_goal_visitor(Vertex goal);
    template<class Graph> void examine_vertex(Vertex u, Graph& /* g */);
private:
    Vertex m_goal;
};

class BoardGraph {
public:
    BoardGraph(int row_num, int col_num);
    ~BoardGraph();

    void remove_edges(const Node &node1, const Node &node2);
    void block_neighbours(const Node &node);
    void unblock_neighbours(const Node &node);
    void get_neighbours(const Node &node, std::set<Node> *node_list) const;

    bool find_path(const Node &start_node, const Node &end_node,
            std::list<Node> *path) const;
    bool is_adjacent(const Node &from_node, const Node &to_node) const;

    void filter_edges(const Node &node1, const Node &node2);
    void reset_filters();
    bool is_path_exists(const Node &node1, const Node &node2) const;

private:
    bool is_adjacent(int from_inode, int to_inode) const;
    bool block_edge(int from_inode, int to_inode);
    bool unblock_edge(int from_inode, int to_inode);

private:
    int row_num_;
    int col_num_;
    graph_t g_;
    FilterEdges fe_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_BOARD_GRAPH_HPP_ */
