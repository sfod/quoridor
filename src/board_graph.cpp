#include "board_graph.hpp"
#include "exception.hpp"

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

BoardGraph::BoardGraph(int row_num, int col_num)
    : g_(), row_num_(row_num), col_num_(col_num), fe_()
{
    if ((row_num <= 0) || (col_num <= 0)) {
        throw Exception("invalid size");
    }

    g_ = graph_t(row_num_ * col_num_);

    std::vector<std::pair<int, int>> edges;
    int idx;
    int nidx;
    for (int i = 0; i < row_num; ++i) {
        for (int j = 0; j < col_num; ++j) {
            idx = i * col_num + j;
            if (j != 0) {
                nidx = i * col_num + j - 1;
                edges.push_back(std::pair<int, int>(idx, nidx));
            }
            if (j != col_num - 1) {
                nidx = i * col_num + j + 1;
                edges.push_back(std::pair<int, int>(idx, nidx));
            }
            if (i != 0) {
                nidx = (i - 1) * col_num + j;
                edges.push_back(std::pair<int, int>(idx, nidx));
            }
            if (i != row_num - 1) {
                nidx = (i + 1) * col_num + j;
                edges.push_back(std::pair<int, int>(idx, nidx));
            }
        }
    }

    WeightMap weightmap = boost::get(boost::edge_weight, g_);
    edge_descriptor e;
    bool b;
    for (auto &it : edges) {
        boost::tie(e, b) = boost::add_edge(it.first, it.second, g_);
        weightmap[e] = 1;
    }
}

BoardGraph::~BoardGraph()
{
}

void BoardGraph::remove_edges(const Node &node1, const Node &node2)
{
    int inode1 = node1.row() * col_num_ + node1.col();
    int inode2 = node2.row() * col_num_ + node2.col();
    edge_descriptor ed;
    bool b;

    boost::tie(ed, b) = boost::edge(inode1, inode2, g_);
    if (b) {
        g_.remove_edge(ed);
    }

    boost::tie(ed, b) = boost::edge(inode2, inode1, g_);
    if (b) {
        g_.remove_edge(ed);
    }
}

void BoardGraph::block_node(const Node &node)
{
    int inode = node.row() * col_num_ + node.col();
    int nf = 0;
    std::map<int, int> neighbours;

    if (block_edge(inode - 1, inode)) {
        nf |= 1;
        neighbours[0] = inode - 1;
    }
    if (block_edge(inode + 1, inode)) {
        nf |= 4;
        neighbours[2] = inode + 1;
    }
    if (block_edge(inode - col_num_, inode)) {
        nf |= 2;
        neighbours[1] = inode - col_num_;
    }
    if (block_edge(inode + col_num_, inode)) {
        nf |= 8;
        neighbours[3] = inode + col_num_;
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

void BoardGraph::unblock_node(const Node &node)
{
    int inode = node.row() * col_num_ + node.col();
    int nf = 0;
    std::map<int, int> neighbours;

    if (unblock_edge(inode - 1, inode)) {
        nf |= 1;
        neighbours[0] = inode - 1;
    }
    if (unblock_edge(inode + 1, inode)) {
        nf |= 4;
        neighbours[2] = inode + 1;
    }
    if (unblock_edge(inode - col_num_, inode)) {
        nf |= 2;
        neighbours[1] = inode - col_num_;
    }
    if (unblock_edge(inode + col_num_, inode)) {
        nf |= 8;
        neighbours[3] = inode + col_num_;
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

void BoardGraph::get_out_node_list(const Node &node,
        std::vector<Node> *node_list) const
{
    IndexMap index = get(boost::vertex_index, g_);
    int inode = node.row() * col_num_ + node.col();
    vertex_descriptor v = boost::vertex(inode, g_);
    Node neighbour_node;

    adjacency_iterator it;
    adjacency_iterator it_end;
    for (boost::tie(it, it_end) = boost::adjacent_vertices(v, g_);
            it != it_end; ++ it) {
        neighbour_node.set_row(index[*it] / row_num_);
        neighbour_node.set_col(index[*it] % col_num_);
        node_list->push_back(neighbour_node);
    }
}

bool BoardGraph::find_path(const Node &start_node, const Node &end_node,
        std::list<Node> *path) const
{
    int start_inode = start_node.row() * col_num_ + start_node.col();
    int end_inode = end_node.row() * col_num_ + end_node.col();

    std::vector<vertex_descriptor> p(boost::num_vertices(g_));
    std::vector<int> d(boost::num_vertices(g_));

    vertex_descriptor start = boost::vertex(start_inode, g_);
    vertex_descriptor end = boost::vertex(end_inode, g_);

    Node node;

    try {
        astar_search(g_, start, boost::astar_heuristic<graph_t, int>(),
                boost::predecessor_map(&p[0]).distance_map(&d[0]).visitor(astar_goal_visitor<vertex_descriptor>(end)));
    }
    catch (found_goal fg) {
        for (vertex_descriptor v = end_inode;; v = p[v]) {
            if (p[v] == v)
                break;
            node = Node(v / col_num_, v % col_num_);
            path->push_front(node);
        }
        return true;
    }

    return false;
}

bool BoardGraph::is_adjacent(const Node &from_node, const Node &to_node) const
{
    int from_inode = from_node.row() * col_num_ + from_node.col();
    int to_inode = to_node.row() * col_num_ + to_node.col();
    return is_adjacent(from_inode, to_inode);
}

void BoardGraph::filter_edges(const Node &node1, const Node &node2)
{
    int inode1 = node1.row() * col_num_ + node1.col();
    int inode2 = node2.row() * col_num_ + node2.col();
    edge_descriptor ed;
    bool b;

    boost::tie(ed, b) = boost::edge(inode1, inode2, g_);
    if (b) {
        fe_.add_edge(ed);
    }

    boost::tie(ed, b) = boost::edge(inode2, inode1, g_);
    if (b) {
        fe_.add_edge(ed);
    }
}

void BoardGraph::reset_filters()
{
    fe_.clear();
}

bool BoardGraph::is_path_exists(const Node &start_node, const Node &end_node) const
{
    int start_inode = start_node.row() * col_num_ + start_node.col();
    int end_inode = end_node.row() * col_num_ + end_node.col();

    boost::filtered_graph<graph_t, FilterEdges> fg(g_, fe_);

    std::vector<boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor> p(boost::num_vertices(fg));
    std::vector<int> d(boost::num_vertices(fg));

    boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor start = boost::vertex(start_inode, g_);
    boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor end = boost::vertex(end_inode, g_);

    try {
        astar_search(fg, start, boost::astar_heuristic<boost::filtered_graph<graph_t, FilterEdges>, int>(),
                boost::predecessor_map(&p[0]).distance_map(&d[0]).visitor(astar_goal_visitor<vertex_descriptor>(end)));
    }
    catch (found_goal fg) {
        return true;
    }

    return false;
}

bool BoardGraph::is_adjacent(int from_inode, int to_inode) const
{
    edge_descriptor e;
    bool b;
    boost::tie(e, b) = boost::edge(from_inode, to_inode, g_);
    return b;
}

bool BoardGraph::block_edge(int from_inode, int to_inode)
{
    edge_descriptor ed;
    bool b;

    boost::tie(ed, b) = boost::edge(from_inode, to_inode, g_);
    if (b) {
        g_.remove_edge(ed);
    }

    return b;
}

bool BoardGraph::unblock_edge(int from_inode, int to_inode)
{
    WeightMap weightmap = boost::get(boost::edge_weight, g_);
    edge_descriptor e;
    bool b;
    boost::tie(e, b) = boost::add_edge(from_inode, to_inode, g_);
    weightmap[e] = 1;
    return b;
}

}  /* namespace Quoridor */
