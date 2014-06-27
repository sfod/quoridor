#ifndef QUORIDOR_BOARD_GRAPH_HPP_
#define QUORIDOR_BOARD_GRAPH_HPP_

#include <vector>

#include <boost/graph/astar_search.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/filtered_graph.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/tag.hpp>

#include "node.hpp"

namespace Quoridor {

struct edge_info_t {
    int weight;
    bool is_tmp;
    int interm_inode;
};

typedef boost::adjacency_list<
        boost::listS,
        boost::vecS,
        boost::bidirectionalS,
        boost::no_property,
        edge_info_t
> graph_t;
typedef boost::property_map<graph_t, int edge_info_t::*>::type edge_info_map_t;
typedef boost::property_map<graph_t, int edge_info_t::*>::const_type const_edge_info_map_t;
typedef graph_t::vertex_descriptor vertex_descriptor;
typedef graph_t::edge_descriptor edge_descriptor;

typedef boost::property_map<graph_t, boost::vertex_index_t>::type IndexMap;
typedef boost::graph_traits<graph_t>::adjacency_iterator adjacency_iterator;
typedef boost::graph_traits<graph_t>::out_edge_iterator out_edge_iterator;
typedef boost::graph_traits<graph_t>::in_edge_iterator in_edge_iterator;

class FilterEdges {
public:
    FilterEdges();
    ~FilterEdges();

    void add_edge(const edge_descriptor &e);
    void rm_edge(const edge_descriptor &e);
    bool exists(const edge_descriptor &e) const;
    template <typename EdgeDesc>
    bool operator()(const EdgeDesc &e) const;

private:
    std::set<edge_descriptor> edges_;
};

struct found_goal {}; // exception for termination

template <class Graph, class CostType>
class astar_heuristic : public boost::astar_heuristic<Graph, CostType> {
public:
    typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex;

    astar_heuristic(int col_num, Node goal_node);
    CostType operator()(vertex u);

private:
    int col_num_;
    Node goal_node_;
};

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


struct by_node {};
typedef boost::multi_index_container<
    Node,
    boost::multi_index::indexed_by<
        boost::multi_index::random_access<>,
        boost::multi_index::ordered_unique<
            boost::multi_index::tag<by_node>,
            boost::multi_index::identity<Node>>
    >
> path_t;

struct path_data_t {
    Node start_node;
    Node end_node;
    path_t path;
    size_t len;
    bool is_exists;
};

struct by_node_len {};
struct by_node_node {};
typedef boost::multi_index_container<
    path_data_t,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<by_node_len>,
            boost::multi_index::composite_key<
                path_data_t,
                boost::multi_index::member<
                    path_data_t, Node, &path_data_t::start_node>,
                boost::multi_index::member<
                    path_data_t, size_t, &path_data_t::len>
            >
        >,
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<by_node_node>,
            boost::multi_index::composite_key<
                path_data_t,
                boost::multi_index::member<
                    path_data_t, Node, &path_data_t::start_node>,
                boost::multi_index::member<
                    path_data_t, Node, &path_data_t::end_node>
            >
        >
    >
> path_data_list_t;


class BoardGraph {
public:
    BoardGraph(int row_num, int col_num);
    ~BoardGraph();

    void remove_edges(const Node &node1, const Node &node2);

    void block_node(const Node &node);
    void unblock_node(const Node &node);
    void get_out_node_list(const Node &node, std::vector<Node> *node_list) const;

    size_t shortest_path(const Node &start_node,
            const std::set<Node> &goal_nodes, std::list<Node> *path) const;
    bool find_path(const Node &start_node, const Node &end_node,
            std::list<Node> *path) const;
    bool is_adjacent(const Node &from_node, const Node &to_node) const;

    bool is_path_exists(const Node &start_node, const Node &end_node,
            const std::vector<std::pair<Node, Node>> blocked_edge_list) const;

private:
    bool is_adjacent(int from_inode, int to_inode, bool check_tmp_edges) const;
    void block_inode(int inode);
    void unblock_inode(int inode);
    bool block_edge(int from_inode, int to_inode, bool is_tmp);
    bool unblock_edge(int from_inode, int to_inode, bool is_tmp, int interm_inode = -1);
    void filter_edges(FilterEdges *fe, const Node &node1,
            const Node &node2) const;
    bool is_inode_valid(int inode) const;

private:
    void add_path_to_cache(const Node &start_node, const Node &end_node,
            const std::list<Node> &path, bool is_exists) const;
    size_t cached_shortest_path(const Node &start_node,
            const std::set<Node> &goal_nodes, std::list<Node> *path) const;
    bool cached_path(const Node &start_node, const Node &end_node,
            std::list<Node> *path) const;
    void update_cached_path(const Node &node1, const Node &node2) const;

private:
    graph_t g_;
    int row_num_;
    int col_num_;
    FilterEdges fe_;
    mutable path_data_list_t path_data_list_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_BOARD_GRAPH_HPP_ */
