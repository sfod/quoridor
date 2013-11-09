#include <boost/graph/astar_search.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random.hpp>
#include <boost/random.hpp>
#include <boost/graph/graphviz.hpp>
#include <sys/time.h>
#include <vector>
#include <list>
#include <iostream>

struct pos_t {
    int row;
    int col;
};

// euclidean distance heuristic
template <class Graph, class CostType, class LocMap>
class distance_heuristic : public boost::astar_heuristic<Graph, CostType>
{
public:
    typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
    distance_heuristic(LocMap l, Vertex goal)
        : m_location(l), m_goal(goal) {}
    CostType operator()(Vertex u) {
        CostType dx = m_location[m_goal].row - m_location[u].row;
        CostType dy = m_location[m_goal].col - m_location[u].col;
        return ::sqrt(dx * dx + dy * dy);
    }
private:
    LocMap m_location;
    Vertex m_goal;
};


struct found_goal {}; // exception for termination

// visitor that terminates when we find the goal
template <class Vertex>
class astar_goal_visitor : public boost::default_astar_visitor
{
public:
    astar_goal_visitor(Vertex goal) : m_goal(goal) {}
    template<class Graph>
    void examine_vertex(Vertex u, Graph& g) {
        if (u == m_goal)
            throw found_goal();
    }
private:
    Vertex m_goal;
};

int main()
{
    typedef boost::adjacency_list< boost::listS, boost::vecS,
            boost::undirectedS, boost::no_property,
            boost::property<boost::edge_weight_t, int> > mygraph_t;
    typedef boost::property_map<mygraph_t, boost::edge_weight_t>::type WeightMap;
    typedef mygraph_t::vertex_descriptor vertex;
    typedef mygraph_t::edge_descriptor edge_descriptor;
    typedef std::pair<int, int> edge;

    int nodes[81];
    pos_t cells[81];
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            nodes[i * 9 + j] = i * 9 + j;
            cells[i * 9 + j].row = i;
            cells[i * 9 + j].col = j;
        }
    }

    edge edges[288];
    int edge_cnt = 0;
    for (int i = 1; i < 8; ++i) {
        for (int j = 1; j < 8; ++j) {
            edges[edge_cnt++] = edge(nodes[i * 9 + j], nodes[(i - 1) * 9 + j]);
            edges[edge_cnt++] = edge(nodes[i * 9 + j], nodes[(i + 1) * 9 + j]);
            edges[edge_cnt++] = edge(nodes[i * 9 + j], nodes[i * 9 + j - 1]);
            edges[edge_cnt++] = edge(nodes[i * 9 + j], nodes[i * 9 + j + 1]);
        }
    }
    for (int i = 1; i < 8; ++i) {
        edges[edge_cnt++] = edge(nodes[i], nodes[i - 1]);
        edges[edge_cnt++] = edge(nodes[i], nodes[i + 1]);
        edges[edge_cnt++] = edge(nodes[i], nodes[9 + i]);
        edges[edge_cnt++] = edge(nodes[9 * 8 + i], nodes[9 * 8 + i - 1]);
        edges[edge_cnt++] = edge(nodes[9 * 8 + i], nodes[9 * 8 + i + 1]);
        edges[edge_cnt++] = edge(nodes[9 * 8 + i], nodes[9 * 8 + i - 9]);
        edges[edge_cnt++] = edge(nodes[9 * i], nodes[9 * (i - 1)]);
        edges[edge_cnt++] = edge(nodes[9 * i], nodes[9 * (i + 1)]);
        edges[edge_cnt++] = edge(nodes[9 * i], nodes[9 * i + 1]);
        edges[edge_cnt++] = edge(nodes[9 * i + 8], nodes[9 * (i - 1) + 8]);
        edges[edge_cnt++] = edge(nodes[9 * i + 8], nodes[9 * (i + 1) + 8]);
        edges[edge_cnt++] = edge(nodes[9 * i + 8], nodes[9 * i + 7]);
    }
    edges[edge_cnt++] = edge(nodes[0], nodes[1]);
    edges[edge_cnt++] = edge(nodes[0], nodes[9]);
    edges[edge_cnt++] = edge(nodes[8], nodes[7]);
    edges[edge_cnt++] = edge(nodes[8], nodes[17]);
    edges[edge_cnt++] = edge(nodes[72], nodes[73]);
    edges[edge_cnt++] = edge(nodes[72], nodes[63]);
    edges[edge_cnt++] = edge(nodes[80], nodes[79]);
    edges[edge_cnt++] = edge(nodes[80], nodes[71]);

    std::cout << "inserted " << edge_cnt << " edges" << std::endl;

    // for (int i = 0; i < 288; ++i) {
    //     std::cout << "neighbours: " << cells[edges[i].first].row << ":"
    //         << cells[edges[i].first].col
    //         << " <-> " << cells[edges[i].second].row << ":"
    //         << cells[edges[i].second].col << std::endl;
    // }

    int weights[288];
    for (int i = 0; i < 81; ++i) {
        weights[i] = 1;
    }

    mygraph_t g(81);
    WeightMap weightmap = boost::get(boost::edge_weight, g);
    for (size_t i = 0; i < edge_cnt; ++i) {
        edge_descriptor e;
        bool b;
        boost::tie(e, b) = boost::add_edge(edges[i].first, edges[i].second, g);
        weightmap[e] = 1;
    }

    boost::mt19937 gen(time(0));
    vertex start = boost::random_vertex(g, gen);
    vertex goal = boost::random_vertex(g, gen);

    std::cout << "Start vertex: " << cells[start].row << "," << cells[start].col << std::endl;
    std::cout << "Goal vertex: " << cells[goal].row << "," << cells[goal].col << std::endl;

    std::vector<mygraph_t::vertex_descriptor> p(boost::num_vertices(g));
    std::vector<int> d(boost::num_vertices(g));

    try {
        astar_search(g, start,
                distance_heuristic<mygraph_t, int, pos_t*>(cells, goal),
                boost::predecessor_map(&p[0]).distance_map(&d[0]).visitor(astar_goal_visitor<vertex>(goal)));
    }
    catch (found_goal fg) {
        std::list<vertex> shortest_path;
        for (vertex v = goal;; v = p[v]) {
            shortest_path.push_front(v);
            if (p[v] == v)
                break;
        }

        std::cout << "shortes path is ";

        std::list<vertex>::iterator spi = shortest_path.begin();
        std::cout << cells[start].row << ":" << cells[start].col;
        for (++spi; spi != shortest_path.end(); ++spi)
            std::cout << " -> " << cells[*spi].row << ":" << cells[*spi].col;
        std::cout << std::endl << "Total travel time: " << d[goal] << std::endl;
    }

    return 0;
}
