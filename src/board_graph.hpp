#ifndef QUORIDOR_BOARD_GRAPH_HPP_
#define QUORIDOR_BOARD_GRAPH_HPP_

#include <vector>

#include <boost/graph/astar_search.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>

namespace Quoridor {

typedef boost::adjacency_list< boost::listS, boost::vecS,
        boost::undirectedS, boost::no_property,
        boost::property<boost::edge_weight_t, int> > graph_t;
typedef boost::property_map<graph_t, boost::edge_weight_t>::type WeightMap;
typedef graph_t::vertex_descriptor vertex;
typedef graph_t::edge_descriptor edge_descriptor; typedef std::pair<int, int> edge;


class FilterEdges {
public:
    FilterEdges();
    ~FilterEdges();

    void add_edge(const edge &e);
    void clear();
    bool operator()(const edge &e);

private:
    std::set<edge> edges_;
};

struct found_goal {}; // exception for termination

// visitor that terminates when we find the goal
template <class Vertex>
class astar_goal_visitor : public boost::default_astar_visitor
{
public:
    astar_goal_visitor(Vertex goal) : m_goal(goal) {}
    template<class Graph>
    void examine_vertex(Vertex u, Graph& /* g */) {
        if (u == m_goal)
            throw found_goal();
    }
private:
    Vertex m_goal;
};

class BoardGraph {
public:
    BoardGraph(int row_num, int col_num);
    ~BoardGraph();

    void remove_edges(int node1, int node2);
    bool find_path(int node1, int node2, std::list<int> *path) const;
    bool is_neighbours(int node1, int node2) const;

    void filter_edges(int node1, int node2);
    void reset_filters();
    bool is_path_exists(int node1, int node2) const;

private:
    graph_t g_;
    std::vector<int> nodes_;
    std::set<edge> edges_;
    FilterEdges fe_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_BOARD_GRAPH_HPP_ */
