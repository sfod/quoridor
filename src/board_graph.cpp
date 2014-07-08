#include "board_graph.hpp"
#include "logger.hpp"
#include "exception.hpp"

static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

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
    : g_(), row_num_(row_num), col_num_(col_num), fe_(), tmp_edges_()
#ifdef USE_BOARD_GRAPH_CACHE
      , path_data_list_()
#endif
{
    lg.add_attribute("Tag", blattrs::constant<std::string>("board graph"));

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

bool BoardGraph::remove_edges(
        const std::vector<std::pair<Node, Node>> &node_pair_list,
        const std::vector<goal_nodes_t> &goal_nodes_list, bool check_only)
{
    std::vector<std::pair<int, int>> removed_edges;
    std::vector<std::pair<int, std::pair<int, int>>> removed_tmp_edges;
#ifdef USE_BOARD_GRAPH_CACHE
    std::set<Node> affected_nodes;
#endif

    for (auto node_pair : node_pair_list) {
        int inode1 = node_pair.first.row() * col_num_ + node_pair.first.col();
        int inode2 = node_pair.second.row() * col_num_ + node_pair.second.col();

        edge_descriptor e;
        bool b;
        boost::tie(e, b) = boost::edge(inode1, inode2, g_);
        if (b) {
            removed_edges.push_back(std::make_pair(inode1, inode2));
#ifdef USE_BOARD_GRAPH_CACHE
            affected_nodes.insert(node_pair.first);
            affected_nodes.insert(node_pair.second);
#endif
        }
        boost::tie(e, b) = boost::edge(inode2, inode1, g_);
        if (b) {
            removed_edges.push_back(std::make_pair(inode2, inode1));
#ifdef USE_BOARD_GRAPH_CACHE
            affected_nodes.insert(node_pair.first);
            affected_nodes.insert(node_pair.second);
#endif
        }

        // find affected temporary edges
        if (tmp_edges_.count(inode1) > 0) {
            for (auto edge_inodes : tmp_edges_[inode1]) {
                if ((edge_inodes.first == inode2) || (edge_inodes.second == inode2)) {
                    removed_edges.push_back(edge_inodes);
                    removed_tmp_edges.push_back(std::make_pair(inode1, edge_inodes));
#ifdef USE_BOARD_GRAPH_CACHE
                    affected_nodes.insert(Node(
                                edge_inodes.first / col_num_,
                                edge_inodes.first % col_num_));
                    affected_nodes.insert(Node(
                                edge_inodes.second / col_num_,
                                edge_inodes.second % col_num_));
#endif
                }
            }
        }
        if (tmp_edges_.count(inode2) > 0) {
            for (auto edge_inodes : tmp_edges_[inode2]) {
                if ((edge_inodes.first == inode1) || (edge_inodes.second == inode1)) {
                    removed_edges.push_back(edge_inodes);
                    removed_tmp_edges.push_back(std::make_pair(inode2, edge_inodes));
#ifdef USE_BOARD_GRAPH_CACHE
                    affected_nodes.insert(Node(
                                edge_inodes.first / col_num_,
                                edge_inodes.first % col_num_));
                    affected_nodes.insert(Node(
                                edge_inodes.second / col_num_,
                                edge_inodes.second % col_num_));
#endif
                }
            }
        }
    }

    if (!check_paths_to_goal_nodes(goal_nodes_list, removed_edges)) {
        return false;
    }

    if (!check_only) {
        edge_descriptor e;
        bool b;
        for (auto edge_inodes : removed_edges) {
            boost::tie(e, b) = boost::edge(edge_inodes.first, edge_inodes.second, g_);
            if (b) {
                g_.remove_edge(e);
            }
            else {
                throw Exception("graph is inconsistent: edge "
                        + std::to_string(edge_inodes.first) + ":"
                        + std::to_string(edge_inodes.second) + " does not exist");
            }
        }

        for (auto tmp_edge : removed_tmp_edges) {
            tmp_edges_[tmp_edge.first].erase(tmp_edge.second);
            if (tmp_edges_[tmp_edge.first].empty()) {
                tmp_edges_.erase(tmp_edge.first);
            }
        }

#ifdef USE_BOARD_GRAPH_CACHE
        for (const auto &n : affected_nodes) {
            update_cached_path(n);
        }
#endif
    }

    return true;
}

void BoardGraph::block_node(const Node &node)
{
    int inode = node.row() * col_num_ + node.col();
    block_inode(inode);

    std::vector<edge_descriptor> tmp_edges;
    std::vector<int> blocked_inodes;
    find_tmp_edges(inode, &tmp_edges);
    for (auto e : tmp_edges) {
        int interm_inode = g_[e].interm_inode;
        blocked_inodes.push_back(interm_inode);
        tmp_edges_[interm_inode].erase(std::make_pair(boost::source(e, g_), inode));
        if (tmp_edges_[interm_inode].empty()) {
            tmp_edges_.erase(interm_inode);
        }
        block_edge(e, true);
    }
    for (auto inode : blocked_inodes) {
        block_inode(inode);
    }

#ifdef USE_BOARD_GRAPH_CACHE
    std::vector<Node> neighbour_nodes;
    node.neighbours(row_num_, col_num_, &neighbour_nodes);
    for (const Node &n : neighbour_nodes) {
        update_cached_path(n);
    }
#endif
}

void BoardGraph::unblock_node(const Node &node)
{
    int inode = node.row() * col_num_ + node.col();
    unblock_inode(inode);

#ifdef USE_BOARD_GRAPH_CACHE
    remove_cached_path(node);

    std::vector<Node> neighbour_nodes;
    node.neighbours(row_num_, col_num_, &neighbour_nodes);
    for (const Node &n : neighbour_nodes) {
        update_cached_path(n);
    }
#endif
}

void BoardGraph::get_out_node_list(const Node &node,
        std::vector<Node> *node_list) const
{
    IndexMap index = get(boost::vertex_index, g_);
    int inode = node.row() * col_num_ + node.col();
    vertex_descriptor v = boost::vertex(inode, g_);
    vertex_descriptor target_v;
    Node neighbour_node;

    out_edge_iterator it;
    out_edge_iterator it_end;
    for (boost::tie(it, it_end) = boost::out_edges(v, g_);
            it != it_end; ++ it) {
        if (fe_.exists(*it)) {
            continue;
        }
        target_v = boost::target(*it, g_);
        neighbour_node.set_row(index[target_v] / row_num_);
        neighbour_node.set_col(index[target_v] % col_num_);
        node_list->push_back(neighbour_node);
    }
}

size_t BoardGraph::shortest_path(const Node &start_node,
        const std::set<Node> &goal_nodes, std::list<Node> *path) const
{
#ifdef USE_BOARD_GRAPH_CACHE
    size_t len;
    if ((len = cached_shortest_path(start_node, goal_nodes, path)) != 0) {
        return len;
    }
#endif

    size_t min_len = 73;
    std::list<Node> tmp_path;
    for (auto goal_node : goal_nodes) {
        if (find_path(start_node, goal_node, &tmp_path)) {
            if (tmp_path.size() < min_len) {
                min_len = tmp_path.size();
                if (path != NULL) {
                    *path = std::move(tmp_path);
                }
            }
            tmp_path.clear();
        }
    }
    return min_len;
}

bool BoardGraph::find_path(const Node &start_node, const Node &end_node,
        std::list<Node> *path) const
{
    if (start_node == end_node) {
        return true;
    }

#ifdef USE_BOARD_GRAPH_CACHE
    if (cached_path(start_node, end_node, path)) {
        return (path->size() != 0);
    }
#endif

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

#ifdef USE_BOARD_GRAPH_CACHE
        add_path_to_cache(start_node, end_node, *path, true);
#endif
        return true;
    }

#ifdef USE_BOARD_GRAPH_CACHE
    add_path_to_cache(start_node, end_node, *path, false);
#endif
    return false;
}

bool BoardGraph::is_adjacent(const Node &from_node, const Node &to_node) const
{
    int from_inode = from_node.row() * col_num_ + from_node.col();
    int to_inode = to_node.row() * col_num_ + to_node.col();
    return is_adjacent(from_inode, to_inode, true);
}

bool BoardGraph::check_paths_to_goal_nodes(
        const std::vector<goal_nodes_t> &goal_nodes_list,
        const std::vector<std::pair<int, int>> &removed_edges) const
{
    FilterEdges fe;
    for (auto edge_inodes : removed_edges) {
        edge_descriptor e;
        bool b;
        boost::tie(e, b) = boost::edge(edge_inodes.first, edge_inodes.second, g_);
        if (b) {
            fe.add_edge(e);
        }
        else {
            throw Exception("graph is inconsistent: temporary edge "
                    + std::to_string(edge_inodes.first) + ":"
                    + std::to_string(edge_inodes.second) + " does not exist");
        }
    }

    boost::filtered_graph<graph_t, FilterEdges> fg(g_, fe);
    std::vector<boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor> p(boost::num_vertices(fg));
    std::vector<int> d(boost::num_vertices(fg));

    bool res = true;
    for (const goal_nodes_t &goal_nodes : goal_nodes_list) {
        bool is_paths_opened = false;

        int start_inode = goal_nodes.node.row() * col_num_ + goal_nodes.node.col();
        for (const Node &goal_node : *goal_nodes.goal_nodes) {
            int end_inode = goal_node.row() * col_num_ + goal_node.col();
            boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor start = boost::vertex(start_inode, g_);
            boost::filtered_graph<graph_t, FilterEdges>::vertex_descriptor end = boost::vertex(end_inode, g_);

            const_edge_info_map_t edge_info_map = boost::get(&edge_info_t::weight, g_);
            try {
                astar_search(fg, start,
                        astar_heuristic<boost::filtered_graph<graph_t, FilterEdges>, int>(col_num_, goal_node),
                        boost::predecessor_map(&p[0])
                            .distance_map(&d[0])
                            .weight_map(edge_info_map)
                            .visitor(astar_goal_visitor<vertex_descriptor>(end)));
            }
            catch (found_goal &fg) {
                is_paths_opened = true;
                break;
            }
        }

        if (!is_paths_opened) {
            res = false;
            break;
        }
    }

    return res;
}

bool BoardGraph::is_adjacent(int from_inode, int to_inode, bool check_tmp_edges) const
{
    if (!is_inode_valid(from_inode) || !is_inode_valid(to_inode)) {
        return false;
    }

    edge_descriptor e;
    bool b;
    boost::tie(e, b) = boost::edge(from_inode, to_inode, g_);
    if (b && check_tmp_edges && fe_.exists(e)) {
        b = false;
    }
    return b;
}

void BoardGraph::block_inode(int inode)
{
    std::vector<int> neighbours = {
        inode - 1,
        inode - col_num_,
        inode + 1,
        inode + col_num_
    };

    // block all edges to the blocked node
    block_edge(inode - 1, inode, false);
    block_edge(inode + 1, inode, false);
    block_edge(inode - col_num_, inode, false);
    block_edge(inode + col_num_, inode, false);

    for (int i = 0; i < 4; ++i) {
        if (is_adjacent(neighbours[i], inode, false)) {
            // check path across blocked node
            if (is_adjacent(inode, neighbours[(i + 2) % 4], true)) {
                unblock_edge(neighbours[i], neighbours[(i + 2) % 4], true, inode);
            }
            // check path to diagonal nodes
            else {
                if (is_adjacent(inode, neighbours[(i + 1) % 4], true)) {
                    unblock_edge(neighbours[i], neighbours[(i + 1) % 4], true, inode);
                }
                if (is_adjacent(inode, neighbours[(i + 3) % 4], true)) {
                    unblock_edge(neighbours[i], neighbours[(i + 3) % 4], true, inode);
                }
            }
        }
    }
}

void BoardGraph::unblock_inode(int inode)
{
    std::vector<int> neighbours = {
        inode - 1,
        inode - col_num_,
        inode + 1,
        inode + col_num_
    };

    // block all edges to the blocked node
    unblock_edge(inode - 1, inode, false);
    unblock_edge(inode + 1, inode, false);
    unblock_edge(inode - col_num_, inode, false);
    unblock_edge(inode + col_num_, inode, false);

    for (auto edge_inodes : tmp_edges_[inode]) {
        block_edge(edge_inodes.first, edge_inodes.second, true);
    }
    tmp_edges_.erase(inode);
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
        block_edge(e, is_tmp);
    }
    return b;
}

void BoardGraph::block_edge(edge_descriptor e, bool is_tmp)
{
    // edge is permanent, block it temporarily via adding to filtered graph
    if (!is_tmp) {
        fe_.add_edge(e);
    }
    // edge is temporary, remove it from the graph
    else if (g_[e].is_tmp) {
        vertex_descriptor v1 = boost::source(e, g_);
        vertex_descriptor v2 = boost::target(e, g_);
        edge_descriptor tmp_e;
        bool b;
        boost::tie(tmp_e, b) = boost::edge(v1, v2, g_);
        if (b) {
            boost::remove_edge(e, g_);
        }
    }
    // @todo handle this situation
    else {
    }
}

bool BoardGraph::unblock_edge(int from_inode, int to_inode, bool is_tmp, int interm_inode)
{
    if (!is_inode_valid(from_inode) || !is_inode_valid(to_inode)) {
        return false;
    }

    edge_descriptor e;
    bool b;
    boost::tie(e, b) = boost::edge(from_inode, to_inode, g_);
    if (b && !is_tmp) {
        fe_.rm_edge(e);
    }
    else if (!b && is_tmp) {
        boost::tie(e, b) = boost::add_edge(from_inode, to_inode, g_);
        if (b) {
            g_[e].weight = 1;
            g_[e].is_tmp = true;
            g_[e].interm_inode = interm_inode;
            tmp_edges_[g_[e].interm_inode].insert(std::make_pair(from_inode, to_inode));
        }
    }

    return b;
}

bool BoardGraph::is_inode_valid(int inode) const
{
    if ((inode < 0)
            || (static_cast<size_t>(inode) >= boost::num_vertices(g_))) {
        return false;
    }
    return true;
}

void BoardGraph::find_tmp_edges(int inode,
        std::vector<edge_descriptor> *tmp_edges) const
{
    vertex_descriptor v = boost::vertex(inode, g_);
    edge_descriptor e;
    in_edge_iterator it;
    in_edge_iterator it_end;
    for (tie(it, it_end) = boost::in_edges(v, g_); it != it_end; ++it) {
        e = *it;
        if (!g_[e].is_tmp) {
            continue;
        }
        tmp_edges->push_back(e);
    }
}

#ifdef USE_BOARD_GRAPH_CACHE
void BoardGraph::add_path_to_cache(const Node &start_node, const Node &end_node,
        const std::list<Node> &path, bool is_exists) const
{
    cached_path_data_t path_data;
    path_data.start_node = start_node;
    path_data.end_node = end_node;
    if (is_exists) {
        std::copy(path.begin(), path.end(), std::back_inserter(path_data.path));
        path_data.len = path.size();
    }
    else {
        path_data.len = 0;
    }
    path_data.is_exists = is_exists;
    path_data_list_.insert(path_data);
}

size_t BoardGraph::cached_shortest_path(const Node &start_node,
        const std::set<Node> &goal_nodes, std::list<Node> *path) const
{
    path_data_list_t::iterator down_it = path_data_list_.get<by_node_len>()
            .upper_bound(boost::make_tuple(start_node, 0));
    path_data_list_t::iterator up_it = path_data_list_.get<by_node_len>()
            .lower_bound(boost::make_tuple(start_node, 73));

    for (auto &e : boost::make_iterator_range(down_it, up_it)) {
        if (goal_nodes.count(e.end_node) == 0) {
            continue;
        }
        if (path != NULL) {
            std::copy(e.path.begin(), e.path.end(), std::back_inserter(*path));
        }
        return e.len;
    }

    return 0;
}

bool BoardGraph::cached_path(const Node &start_node, const Node &end_node,
        std::list<Node> *path) const
{
    path_data_list_t::index<by_node_node>::type::iterator it =
            path_data_list_.get<by_node_node>()
                    .find(boost::make_tuple(start_node, end_node));
    if (it != path_data_list_.get<by_node_node>().end()) {
        if (it->is_exists) {
            std::copy(it->path.begin(), it->path.end(), std::back_inserter(*path));
        }
        return true;
    }
    return false;
}

void BoardGraph::update_cached_path(const Node &node) const
{
    auto it = path_data_list_.begin();
    while (it != path_data_list_.end()) {
        if (it->path.get<by_node>().find(node) != it->path.get<by_node>().end()) {
            it = path_data_list_.erase(it);
        }
        else {
            ++it;
        }
    }
}

void BoardGraph::remove_cached_path(const Node &node) const
{
    path_data_list_t::iterator it;
    path_data_list_t::iterator it_end;
    boost::tie(it, it_end) = path_data_list_.get<by_node_len>().equal_range(node);
    while (it != it_end) {
        it = path_data_list_.erase(it);
    }
}
#endif

}  /* namespace Quoridor */
