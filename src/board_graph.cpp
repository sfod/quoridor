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

void FilterEdges::rm_edge(const edge_descriptor &e)
{
    edges_.erase(e);
}

bool FilterEdges::exists(const edge_descriptor &e) const
{
    return edges_.count(e) > 0;
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

template <class Graph, class CostType>
astar_heuristic<Graph, CostType>::astar_heuristic(int col_num, Node goal_node)
    : col_num_(col_num), goal_node_(goal_node)
{
}

template <class Graph, class CostType>
CostType astar_heuristic<Graph, CostType>::operator()(typename boost::graph_traits<Graph>::vertex_descriptor u)
{
    Node n(u / col_num_, u % col_num_);

    CostType dx = goal_node_.col() - n.col();
    CostType dy = goal_node_.row() - n.row();
    return std::sqrt(dx * dx + dy * dy);
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

    edge_descriptor e;
    bool b;
    for (auto &it : edges) {
        boost::tie(e, b) = boost::add_edge(it.first, it.second, g_);
        g_[e].weight = 1;
        g_[e].is_tmp = false;
    }
}

BoardGraph::~BoardGraph()
{
}

void BoardGraph::remove_edges(const Node &node1, const Node &node2)
{
    int inode1 = node1.row() * col_num_ + node1.col();
    int inode2 = node2.row() * col_num_ + node2.col();
    edge_descriptor e;
    bool b;

    boost::tie(e, b) = boost::edge(inode1, inode2, g_);
    if (b) {
        g_.remove_edge(e);
    }

    boost::tie(e, b) = boost::edge(inode2, inode1, g_);
    if (b) {
        g_.remove_edge(e);
    }
}

void BoardGraph::block_node(const Node &node)
{
    int inode = node.row() * col_num_ + node.col();
    block_inode(inode);

    IndexMap index = get(boost::vertex_index, g_);
    std::set<int> neighbours;
    std::set<std::pair<int, int>> tmp_edges;
    vertex_descriptor v = boost::vertex(inode, g_);
    edge_descriptor e;
    vertex_descriptor source_v;
    vertex_descriptor target_v;
    in_edge_iterator it;
    in_edge_iterator it_end;
    for (tie(it, it_end) = boost::in_edges(v, g_); it != it_end; ++it) {
        e = *it;
        if (!g_[e].is_tmp) {
            continue;
        }
        source_v = boost::source(e, g_);
        target_v = boost::target(e, g_);
        tmp_edges.insert(std::make_pair(source_v, target_v));
        neighbours.insert(g_[e].interm_inode);
    }

    for (auto tmp_edge : tmp_edges) {
        block_edge(index[tmp_edge.first], index[tmp_edge.second], true);
    }

    for (int neighbour_inode : neighbours) {
        block_inode(neighbour_inode);
    }
}

void BoardGraph::unblock_node(const Node &node)
{
    int inode = node.row() * col_num_ + node.col();
    unblock_inode(inode);
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

    boost::filtered_graph<graph_t, FilterEdges> fg(g_, fe_);

    std::vector<boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor> p(boost::num_vertices(fg));
    std::vector<int> d(boost::num_vertices(fg));

    boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor start = boost::vertex(start_inode, g_);
    boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor end = boost::vertex(end_inode, g_);

    Node node;

    const_edge_info_map_t edge_info_map = boost::get(&edge_info_t::weight, g_);
    try {
        astar_search(fg, start,
                astar_heuristic<boost::filtered_graph<graph_t, FilterEdges>, int>(col_num_, end_node),
                boost::predecessor_map(&p[0])
                    .distance_map(&d[0])
                    .weight_map(edge_info_map)
                    .visitor(astar_goal_visitor<vertex_descriptor>(end)));
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

void BoardGraph::reset_filters()
{
    fe_.clear();
}

bool BoardGraph::is_path_exists(const Node &start_node, const Node &end_node,
        const std::vector<std::pair<Node, Node>> blocked_edge_list) const
{
    FilterEdges fe;
    for (auto blocked_edge : blocked_edge_list) {
        filter_edges(&fe, blocked_edge.first, blocked_edge.second);
    }

    int start_inode = start_node.row() * col_num_ + start_node.col();
    int end_inode = end_node.row() * col_num_ + end_node.col();

    boost::filtered_graph<graph_t, FilterEdges> fg(g_, fe);

    std::vector<boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor> p(boost::num_vertices(fg));
    std::vector<int> d(boost::num_vertices(fg));

    boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor start = boost::vertex(start_inode, g_);
    boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor end = boost::vertex(end_inode, g_);

    const_edge_info_map_t edge_info_map = boost::get(&edge_info_t::weight, g_);
    try {
        astar_search(fg, start,
                astar_heuristic<boost::filtered_graph<graph_t, FilterEdges>, int>(col_num_, end_node),
                boost::predecessor_map(&p[0])
                    .distance_map(&d[0])
                    .weight_map(edge_info_map)
                    .visitor(astar_goal_visitor<vertex_descriptor>(end)));
    }
    catch (found_goal &fg) {
        return true;
    }

    return false;
}

bool BoardGraph::is_adjacent(int from_inode, int to_inode) const
{
    if (!is_inode_valid(from_inode) || !is_inode_valid(to_inode)) {
        return false;
    }

    edge_descriptor e;
    bool b;
    boost::tie(e, b) = boost::edge(from_inode, to_inode, g_);
    if (b && fe_.exists(e)) {
        b = false;
    }
    return b;
}

void BoardGraph::block_inode(int inode)
{
    int nf = 0;
    std::map<int, int> neighbours;

    // block all edges to the blocked node
    block_edge(inode - 1, inode, false);
    block_edge(inode + 1, inode, false);
    block_edge(inode - col_num_, inode, false);
    block_edge(inode + col_num_, inode, false);

    if (is_adjacent(inode, inode - 1)) {
        nf |= 1;
        neighbours[0] = inode - 1;
    }
    if (is_adjacent(inode, inode + 1)) {
        nf |= 4;
        neighbours[2] = inode + 1;
    }
    if (is_adjacent(inode, inode - col_num_)) {
        nf |= 2;
        neighbours[1] = inode - col_num_;
    }
    if (is_adjacent(inode, inode + col_num_)) {
        nf |= 8;
        neighbours[3] = inode + col_num_;
    }

    // link neighbours with each other
    for (int i = 0; i < 4; ++i) {
        if (nf & (1 << i)) {
            // path to the opposite node is open
            if (nf & (1 << ((i + 2) % 4))) {
                unblock_edge(neighbours[i], neighbours[(i + 2) % 4], true, inode);
            }
            // path to the opposite node is blocked, open pathes to diagonal nodes
            else {
                if (nf & (1 << ((i + 1) % 4))) {
                    unblock_edge(neighbours[i], neighbours[(i + 1) % 4], true, inode);
                }
                if (nf & (1 << ((i + 3) % 4))) {
                    unblock_edge(neighbours[i], neighbours[(i + 3) % 4], true, inode);
                }
            }
        }
    }
}

void BoardGraph::unblock_inode(int inode)
{
    int nf = 0;
    std::map<int, int> neighbours;

    unblock_edge(inode - 1, inode, false);
    unblock_edge(inode + 1, inode, false);
    unblock_edge(inode - col_num_, inode, false);
    unblock_edge(inode + col_num_, inode, false);

    if (is_adjacent(inode, inode - 1)) {
        nf |= 1;
        neighbours[0] = inode - 1;
    }
    if (is_adjacent(inode, inode + 1)) {
        nf |= 4;
        neighbours[2] = inode + 1;
    }
    if (is_adjacent(inode, inode - col_num_)) {
        nf |= 2;
        neighbours[1] = inode - col_num_;
    }
    if (is_adjacent(inode, inode + col_num_)) {
        nf |= 8;
        neighbours[3] = inode + col_num_;
    }

    // close links between node's neighbours
    for (int i = 0; i < 4; ++i) {
        if (nf & (1 << i)) {
            // path to the opposite node is open
            if (nf & (1 << ((i + 2) % 4))) {
                block_edge(neighbours[i], neighbours[(i + 2) % 4], true);
            }
            // path to the opposite node is blocked, open pathes to diagonal nodes
            else {
                if (nf & (1 << ((i + 1) % 4))) {
                    block_edge(neighbours[i], neighbours[(i + 1) % 4], true);
                }
                if (nf & (1 << ((i + 3) % 4))) {
                    block_edge(neighbours[i], neighbours[(i + 3) % 4], true);
                }
            }
        }
    }
}

bool BoardGraph::block_edge(int from_inode, int to_inode, bool is_tmp)
{
    if (!is_inode_valid(from_inode) || !is_inode_valid(to_inode)) {
        return false;
    }

    edge_descriptor e;
    bool b;
    boost::tie(e, b) = boost::edge(from_inode, to_inode, g_);
    if (b) {
        // edge is permanent, block it temporarily via adding to filtered graph
        if (!is_tmp) {
            fe_.add_edge(e);
        }
        // edge is temporary, remove it from the graph
        else if (g_[e].is_tmp) {
            boost::remove_edge(e, g_);
        }
        // @todo handle this situation
        else {
        }
    }
    return b;
}

bool BoardGraph::unblock_edge(int from_inode, int to_inode, bool is_tmp, int interm_inode)
{
    if (!is_inode_valid(from_inode) || !is_inode_valid(to_inode)) {
        return false;
    }

    edge_descriptor e;
    bool b;

    if (is_tmp) {
        boost::tie(e, b) = boost::add_edge(from_inode, to_inode, g_);
        if (b) {
            g_[e].weight = 1;
            g_[e].is_tmp = true;
            g_[e].interm_inode = interm_inode;
        }
        // @todo handle
        else {
        }
    }
    else {
        boost::tie(e, b) = boost::edge(from_inode, to_inode, g_);
        if (b) {
            fe_.rm_edge(e);
        }
    }

    return b;
}

void BoardGraph::filter_edges(FilterEdges *fe, const Node &node1,
        const Node &node2) const
{
    int inode1 = node1.row() * col_num_ + node1.col();
    int inode2 = node2.row() * col_num_ + node2.col();
    edge_descriptor e;
    bool b;

    boost::tie(e, b) = boost::edge(inode1, inode2, g_);
    if (b) {
        fe->add_edge(e);
    }

    boost::tie(e, b) = boost::edge(inode2, inode1, g_);
    if (b) {
        fe->add_edge(e);
    }
}

bool BoardGraph::is_inode_valid(int inode) const
{
    if ((inode < 0)
            || (static_cast<size_t>(inode) >= boost::num_vertices(g_))) {
        return false;
    }
    return true;
}

}  /* namespace Quoridor */
