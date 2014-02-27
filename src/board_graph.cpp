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

template <class Vertex>
astar_goal_visitor<Vertex>::astar_goal_visitor(Vertex goal) : m_goal(goal)
{
}

template <class Vertex>
template <class Graph>
void astar_goal_visitor<Vertex>::examine_vertex(Vertex u, Graph & /* g */)
{
    if (u == m_goal) {
        throw found_goal();
    }
}

BoardGraph::BoardGraph() : row_num_(0), col_num_(0), g_(), nodes_(), edges_(), fe_()
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

    row_num_ = row_num;
    col_num_ = col_num;

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

void BoardGraph::block_edge(int node1, int node2)
{
    edge_descriptor ed;
    bool b;

    boost::tie(ed, b) = boost::edge(node1, node2, g_);
    if (b) {
        g_.remove_edge(ed);
    }
}

void BoardGraph::unblock_edge(int node1, int node2)
{
    WeightMap weightmap = boost::get(boost::edge_weight, g_);
    edge_descriptor e;
    bool b;

    boost::tie(e, b) = boost::add_edge(node1, node2, g_);
    weightmap[e] = 1;
}

void BoardGraph::block_neighbours(int node)
{
    int nf = 0;
    std::map<int, int> neighbours;

    if (is_neighbours(node, node - 1)) {
        block_edge(node - 1, node);
        nf |= 1;
        neighbours[0] = node - 1;
    }
    if (is_neighbours(node, node + 1)) {
        block_edge(node + 1, node);
        nf |= 4;
        neighbours[2] = node + 1;
    }
    if (is_neighbours(node, node - col_num_)) {
        block_edge(node - col_num_, node);
        nf |= 2;
        neighbours[1] = node - col_num_;
    }
    if (is_neighbours(node, node + col_num_)) {
        block_edge(node + col_num_, node);
        nf |= 8;
        neighbours[3] = node + col_num_;
    }

    // link neighbours with each other
    for (int i = 0; i < 4; ++i) {
        if (nf & (1 << i)) {
            // path to the opposite node is open
            if (nf & (1 << ((i + 2) % 4))) {
                unblock_edge(neighbours[i], neighbours[(i + 2) % 4]);
            }
            // path to the opposite node is blocked, open pathes to diagonal nodes
            else {
                if (nf & (1 << ((i + 1) % 4))) {
                    unblock_edge(neighbours[i], neighbours[(i + 1) % 4]);
                }
                if (nf & (1 << ((i + 3) % 4))) {
                    unblock_edge(neighbours[i], neighbours[(i + 3) % 4]);
                }
            }
        }
    }
}

void BoardGraph::unblock_neighbours(int node)
{
    int nf = 0;
    std::map<int, int> neighbours;

    if (is_neighbours(node, node - 1)) {
        unblock_edge(node - 1, node);
        nf |= 1;
        neighbours[0] = node - 1;
    }
    if (is_neighbours(node, node + 1)) {
        unblock_edge(node + 1, node);
        nf |= 4;
        neighbours[2] = node + 1;
    }
    if (is_neighbours(node, node - col_num_)) {
        unblock_edge(node - col_num_, node);
        nf |= 2;
        neighbours[1] = node - col_num_;
    }
    if (is_neighbours(node, node + col_num_)) {
        unblock_edge(node + col_num_, node);
        nf |= 8;
        neighbours[3] = node + col_num_;
    }

    // close links between node's neighbours
    for (int i = 0; i < 4; ++i) {
        if (nf & (1 << i)) {
            // path to the opposite node is open
            if (nf & (1 << ((i + 2) % 4))) {
                block_edge(neighbours[i], neighbours[(i + 2) % 4]);
            }
            // path to the opposite node is blocked, open pathes to diagonal nodes
            else {
                if (nf & (1 << ((i + 1) % 4))) {
                    block_edge(neighbours[i], neighbours[(i + 1) % 4]);
                }
                if (nf & (1 << ((i + 3) % 4))) {
                    block_edge(neighbours[i], neighbours[(i + 3) % 4]);
                }
            }
        }
    }
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
            if (p[v] == v)
                break;
            path->push_front(v);
        }
        return true;
    }

    return false;
}

bool BoardGraph::is_neighbours(int node1, int node2) const
{
    return (edges_.count(edge(node1, node2)) > 0);
}

bool BoardGraph::is_adjacent(int node1, int node2) const
{
    IndexMap index = get(boost::vertex_index, g_);
    std::pair<adjacency_iterator, adjacency_iterator> neighbours =
        boost::adjacent_vertices(boost::vertex(node1, g_), g_);
    for (; neighbours.first != neighbours.second; ++ neighbours.first) {
        if ((int) index[*neighbours.first] == node2) {
            return true;
        }
    }
    return false;
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
