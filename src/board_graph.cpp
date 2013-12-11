#include "board_graph.hpp"

namespace Quoridor {

FilterEdges::FilterEdges() : edges_()
{
}

FilterEdges::~FilterEdges()
{
}

void FilterEdges::add_edge(const edge_descriptor &e)
{
    edges_.insert(e);
}

void FilterEdges::clear()
{
    edges_.clear();
}

template <typename EdgeDesc>
bool FilterEdges::operator()(const EdgeDesc &e) const
{
    return edges_.find(e) == edges_.end();
}

BoardGraph::BoardGraph() : g_(), nodes_(), edges_(), fe_()
{
}

BoardGraph::~BoardGraph()
{
}

int BoardGraph::set_size(int row_num, int col_num)
{
    if ((row_num <= 0) || (col_num <= 0)) {
        return -1;
    }

    g_ = graph_t(row_num * col_num);

    for (int i = 0; i < row_num * col_num; ++i) {
        nodes_.push_back(i);
    }

    int nidx;
    for (int i = 0; i < row_num; ++i) {
        for (int j = 0; j < col_num; ++j) {
            if (j != 0) {
                nidx = i * col_num + j - 1;
                edges_.insert(edge(nodes_[i * col_num + j], nodes_[nidx]));
            }
            if (j != col_num - 1) {
                nidx = i * col_num + j + 1;
                edges_.insert(edge(nodes_[i * col_num + j], nodes_[nidx]));
            }
            if (i != 0) {
                nidx = (i - 1) * col_num + j;
                edges_.insert(edge(nodes_[i * col_num + j], nodes_[nidx]));
            }
            if (i != row_num - 1) {
                nidx = (i + 1) * col_num + j;
                edges_.insert(edge(nodes_[i * col_num + j], nodes_[nidx]));
            }
        }
    }

    WeightMap weightmap = boost::get(boost::edge_weight, g_);
    edge_descriptor e;
    bool b;
    for (auto &it : edges_) {
        boost::tie(e, b) = boost::add_edge(it.first, it.second, g_);
        weightmap[e] = 1;
    }

    return 0;
}

void BoardGraph::remove_edges(int node1, int node2)
{
    edge_descriptor ed;
    bool b;

    boost::tie(ed, b) = boost::edge(node1, node2, g_);
    if (b) {
        g_.remove_edge(ed);
    }

    boost::tie(ed, b) = boost::edge(node2, node1, g_);
    if (b) {
        g_.remove_edge(ed);
    }

    edges_.erase(edge(node1, node2));
    edges_.erase(edge(node2, node1));
}

bool BoardGraph::find_path(int start_node, int end_node, std::list<int> *path) const
{
    std::vector<vertex> p(boost::num_vertices(g_));
    std::vector<int> d(boost::num_vertices(g_));

    vertex start = boost::vertex(start_node, g_);
    vertex end = boost::vertex(end_node, g_);

    try {
        astar_search(g_, start, boost::astar_heuristic<graph_t, int>(),
                boost::predecessor_map(&p[0]).distance_map(&d[0]).visitor(astar_goal_visitor<vertex>(end)));
    }
    catch (found_goal fg) {
        for (vertex v = end_node;; v = p[v]) {
            path->push_front(v);
            if (p[v] == v)
                break;
        }

        std::cout << "shortes path is ";

        std::list<int>::iterator spi = path->begin();
        std::cout << (start_node / 9) << ":" << (start_node % 9);
        for (++spi; spi != path->end(); ++spi)
            std::cout << " -> " << (nodes_[*spi] / 9) << ":"
                << (nodes_[*spi] % 9);
        std::cout << std::endl << "Total travel time: " << d[end_node]
            << std::endl;

        return true;
    }

    return false;
}

bool BoardGraph::is_neighbours(int node1, int node2) const
{
    return (edges_.count(edge(node1, node2)) > 0);
}

void BoardGraph::filter_edges(int node1, int node2)
{
    edge_descriptor ed;
    bool b;

    boost::tie(ed, b) = boost::edge(node1, node2, g_);
    if (b) {
        fe_.add_edge(ed);
    }

    boost::tie(ed, b) = boost::edge(node2, node1, g_);
    if (b) {
        fe_.add_edge(ed);
    }
}

void BoardGraph::reset_filters()
{
    fe_.clear();
}

bool BoardGraph::is_path_exists(int start_node, int end_node) const
{
    boost::filtered_graph<graph_t, FilterEdges> fg(g_, fe_);

    std::vector<boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor> p(boost::num_vertices(fg));
    std::vector<int> d(boost::num_vertices(fg));

    boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor start = boost::vertex(start_node, g_);
    boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor end = boost::vertex(end_node, g_);

    try {
        astar_search(fg, start, boost::astar_heuristic<boost::filtered_graph<graph_t, FilterEdges>, int>(),
                boost::predecessor_map(&p[0]).distance_map(&d[0]).visitor(astar_goal_visitor<vertex>(end)));
    }
    catch (found_goal fg) {
        return true;
    }

    return false;
}

}  /* namespace Quoridor */
