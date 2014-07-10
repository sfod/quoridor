#include "game.hpp"
#include "exception.hpp"

namespace Quoridor {

// order in which pawns are adding
// pawns rotate according to their indexes: 0 -> 1 -> 2 -> 3 -> 0 -> ...
static const std::vector<int> pawn_idx_list = {0, 2, 1, 3};

Game::Game(int row_num, int col_num) : row_num_(row_num), col_num_(col_num),
    pawn_data_list_(), cur_pawn_idx_(-1), bg_(row_num, col_num),
    wg_(row_num + 1)
{
}

Game::~Game()
{
}

void Game::set_pawns(std::vector<std::shared_ptr<Pawn>> &pawn_list)
{
    size_t pawn_num = pawn_list.size();
    if ((pawn_num != 2) && (pawn_num != 4)) {
        throw Exception("Invalid number of players: " + std::to_string(pawn_num));
    }

    int wall_num = 20 / pawn_list.size();
    for (size_t i = 0; i < pawn_list.size(); ++i) {
        pawn_data_t pawn_data;
        pawn_data.pawn = pawn_list[i];
        pawn_data.wall_num = wall_num;

        pawn_data.idx = pawn_idx_list[i];
        switch (pawn_data.idx) {
        case 0:
            pawn_data.node.set_row(0);
            pawn_data.node.set_col(col_num_ / 2);
            bg_.block_node(pawn_data.node);
            for (int j = 0; j < col_num_; ++j) {
                Node gn(row_num_ - 1, j);
                pawn_data.goal_nodes.insert(gn);
            }
            break;
        case 1:
            pawn_data.node.set_row(row_num_ / 2);
            pawn_data.node.set_col(0);
            bg_.block_node(pawn_data.node);
            for (int j = 0; j < row_num_; ++j) {
                Node gn(j, col_num_ - 1);
                pawn_data.goal_nodes.insert(gn);
            }
            break;
        case 2:
            pawn_data.node.set_row(row_num_ - 1);
            pawn_data.node.set_col(col_num_ / 2);
            bg_.block_node(pawn_data.node);
            for (int j = 0; j < col_num_; ++j) {
                Node gn(0, j);
                pawn_data.goal_nodes.insert(gn);
            }
            break;
        case 3:
            pawn_data.node.set_row(row_num_ / 2);
            pawn_data.node.set_col(col_num_ - 1);
            bg_.block_node(pawn_data.node);
            for (int j = 0; j < row_num_; ++j) {
                Node gn(j, 0);
                pawn_data.goal_nodes.insert(gn);
            }
            break;
        }

        pawn_data_list_.insert(pawn_data);
    }
}

void Game::switch_pawn()
{
    auto it = pawn_data_list_.upper_bound(cur_pawn_idx_);
    if (it == pawn_data_list_.end()) {
        it = pawn_data_list_.begin();
    }
    cur_pawn_idx_ = it->idx;
}

const pawn_data_t &Game::cur_pawn_data() const
{
    return *pawn_data_list_.find(cur_pawn_idx_);
}

const pawn_data_list_t &Game::pawn_data_list() const
{
    return pawn_data_list_;
}

const pawn_data_t &Game::pawn_data(const std::shared_ptr<Pawn> &pawn) const
{
    return *pawn_data_list_.get<by_pawn>().find(pawn);
}

int Game::move_pawn(const Node &node)
{
    const Node &cur_node = pawn_data_list_.find(cur_pawn_idx_)->node;

    if (bg_.is_adjacent(cur_node, node)) {
        bg_.unblock_node(cur_node);
        pawn_data_list_t::iterator it = pawn_data_list_.find(cur_pawn_idx_);
        pawn_data_list_.modify(it, [=](pawn_data_t &e){ e.node = node; });
        bg_.block_node(node);
        return 0;
    }
    else {
        return -1;
    }
}

int Game::add_wall(const Wall &wall)
{
    if (pawn_data_list_.find(cur_pawn_idx_)->wall_num == 0) {
        return -1;
    }

    if (wg_.add_tmp_wall(wall) < 0) {
        return -1;
    }

    std::vector<std::pair<Node, Node>> edge_list;

    Node node1;
    Node node2;
    Node node_tmp;
    if (wall.orientation() == Wall::kHorizontal) {
        for (int i = 0; i < wall.cnt(); ++i) {
            node1 = Node(wall.row() - 1, wall.col() + i);
            node2 = Node(wall.row(), wall.col() + i);
            edge_list.push_back(std::make_pair(node1, node2));
        }
    }
    else if (wall.orientation() == Wall::kVertical) {
        for (int i = 0; i < wall.cnt(); ++i) {
            node1 = Node(wall.row() + i, wall.col() - 1);
            node2 = Node(wall.row() + i, wall.col());
            edge_list.push_back(std::make_pair(node1, node2));
        }
    }
    else {
        return -1;
    }

    std::vector<goal_nodes_t> goal_nodes_list;
    for (const auto &pawn_data : pawn_data_list_) {
        goal_nodes_t gn;
        gn.node = pawn_data.node;
        gn.goal_nodes = &pawn_data.goal_nodes;
        goal_nodes_list.push_back(gn);
    }

    if (!bg_.remove_edges(edge_list, goal_nodes_list, false)) {
        return -1;
    }

    wg_.apply_tmp_wall();

    pawn_data_list_t::iterator it = pawn_data_list_.find(cur_pawn_idx_);
    pawn_data_list_.modify(it, [=](pawn_data_t &e){ --e.wall_num; });

    return 0;
}

bool Game::is_finished() const
{
    Node cur_node = pawn_data_list_.find(cur_pawn_idx_)->node;
    if (pawn_data_list_.find(cur_pawn_idx_)->goal_nodes.count(cur_node) != 0) {
        return true;
    }
    return false;
}

size_t Game::shortest_path(const Node &start_node,
        const std::set<Node> &goal_nodes, std::list<Node> *path) const
{
    return bg_.shortest_path(start_node, goal_nodes, path);
}

std::vector<move_t> Game::possible_moves(std::shared_ptr<Pawn> pawn) const
{
    std::vector<move_t> moves;

    const pawn_data_t &pawn_data = *pawn_data_list_.get<by_pawn>().find(pawn);
    std::vector<Node> nodes = bg_.adjacent_nodes(pawn_data.node);
    std::vector<std::pair<Node, size_t>> node_pathes;
    for (auto node : nodes) {
        node_pathes.push_back(std::make_pair(node, shortest_path(node, pawn_data.goal_nodes, NULL)));
    }

    std::sort(node_pathes.begin(), node_pathes.end(),
            [](std::pair<Node, size_t> a, std::pair<Node, size_t> b) {
                return a.second < b.second;
            }
    );

    for (auto p : node_pathes) {
        moves.push_back(p.first);
    }

    if (pawn_data.wall_num > 0) {
        std::vector<Wall> walls;
        wg_.possible_walls(&walls);
        for (auto wall : walls) {
            moves.push_back(wall);
        }
    }

    return moves;
}

}  // namespace Quoridor
