#ifndef QUORIDOR_BOARD_GRAPH_HPP_
#define QUORIDOR_BOARD_GRAPH_HPP_

#include <vector>

#include <boost/graph/astar_search.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/filtered_graph.hpp>

namespace Quoridor {

typedef boost::adjacency_list< boost::listS, boost::vecS,
        boost::undirectedS, boost::no_property,
        boost::property<boost::edge_weight_t, int> > graph_t;
typedef boost::property_map<graph_t, boost::edge_weight_t>::type WeightMap;
typedef graph_t::vertex_descriptor vertex;
typedef graph_t::edge_descriptor edge_descriptor;
typedef std::pair<int, int> edge;

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

struct found_goal {
    found_goal() : i(0) {
        ++i;
    }
    ~found_goal() {
        --i;
    }

    int i;
}; // exception for termination

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
    BoardGraph();
    ~BoardGraph();

    int set_size(int row_num, int col_num);

    void remove_edges(int node1, int node2);
    void block_edge(int node1, int node2);
    void unblock_edge(int node1, int node2);
    void block_neighbours(int node);
    void unblock_neighbours(int node);

    bool find_path(int node1, int node2, std::list<int> *path) const;
    bool is_neighbours(int node1, int node2) const;
    bool is_adjacent(int node1, int node2) const;

    void filter_edges(int node1, int node2);
    void reset_filters();
    bool is_path_exists(int node1, int node2) const;

private:
    graph_t g_;
    std::vector<int> nodes_;
    std::set<edge> edges_;
    std::map<int, std::set<int>> neighbours_;
    FilterEdges fe_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_BOARD_GRAPH_HPP_ */
