#include "game.hpp"
#include "exception.hpp"

namespace Quoridor {

// order in which pawns are adding
// pawns rotates according to their indexes: 0 -> 1 -> 2 -> 3 -> 0 -> ...
static const std::vector<int> pawn_idx_list = {0, 2, 1, 3};

Game::Game(int board_size) : board_size_(board_size), pawn_data_list_(),
    cur_pawn_idx_(-1), bg_(board_size_, board_size_), wg_(board_size_ + 1)
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


    for (size_t i = 0; i < pawn_list.size(); ++i) {
        pawn_data_t pawn_data;
        pawn_data.pawn = pawn_list[i];

        pawn_data.idx = pawn_idx_list[i];
        switch (pawn_data.idx) {
        case 0:
            pawn_data.node.set_row(0);
            pawn_data.node.set_col(board_size_ / 2);
            for (int j = 0; j < board_size_; ++j) {
                Node gn(board_size_ - 1, j);
                pawn_data.goal_nodes.insert(gn);
            }
            break;
        case 1:
            pawn_data.node.set_row(board_size_ / 2);
            pawn_data.node.set_col(0);
            for (int j = 0; j < board_size_; ++j) {
                Node gn(j, board_size_ - 1);
                pawn_data.goal_nodes.insert(gn);
            }
            break;
        case 2:
            pawn_data.node.set_row(board_size_ - 1);
            pawn_data.node.set_col(board_size_ / 2);
            for (int j = 0; j < board_size_; ++j) {
                Node gn(0, j);
                pawn_data.goal_nodes.insert(gn);
            }
            break;
        case 3:
            pawn_data.node.set_row(board_size_ / 2);
            pawn_data.node.set_col(board_size_ - 1);
            for (int j = 0; j < board_size_; ++j) {
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

const pawn_data_t &Game::pawn_data(std::shared_ptr<Pawn> &pawn) const
{
    return *pawn_data_list_.get<by_pawn>().find(pawn);
}

int Game::move_pawn(const Node &node)
{
    const Node &cur_node = pawn_data_list_.find(cur_pawn_idx_)->node;

    if (bg_.is_adjacent(cur_node, node)) {
        bg_.unblock_neighbours(cur_node);
        pawn_data_list_t::iterator it = pawn_data_list_.find(cur_pawn_idx_);
        pawn_data_list_.modify(it, [=](pawn_data_t &e){ e.node = node; });
        bg_.block_neighbours(node);
        return 0;
    }
    else {
        return -1;
    }
}

int Game::add_wall(const Wall &wall)
{
    std::vector<std::pair<Node, Node>> edges;

    if (try_add_wall(wall, &edges) < 0) {
        return -1;
    }

    wg_.apply_tmp_wall();
    for (auto edge : edges) {
        bg_.remove_edges(edge.first, edge.second);
    }

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

bool Game::get_path(std::shared_ptr<Pawn> pawn, const Node &node,
        std::list<Node> *path) const
{
    return bg_.find_path(pawn_data(pawn).node, node, path);
}

void Game::possible_moves(std::shared_ptr<Pawn> pawn, std::set<Node> *nodes,
        std::vector<Wall> *walls) const
{
    bg_.get_neighbours(pawn_data_list_.get<by_pawn>().find(pawn)->node, nodes);
    wg_.possible_walls(walls);
}

int Game::try_add_wall(const Wall &wall,
        std::vector<std::pair<Node, Node>> *edges)
{
    if (wg_.add_tmp_wall(wall) < 0) {
        return -1;
    }

    bg_.reset_filters();

    Node node1;
    Node node2;
    Node node_tmp;
    if (wall.orientation() == Wall::kHorizontal) {
        for (int i = 0; i < wall.cnt(); ++i) {
            node1 = Node(wall.row() - 1, wall.col() + i);
            node2 = Node(wall.row(), wall.col() + i);
            bg_.filter_edges(node1, node2);
            edges->push_back(std::make_pair(node1, node2));

            node_tmp = Node(wall.row() + 1, wall.col() + i);
            if ((node_tmp.row() >= 0) && (node_tmp.row() < board_size_)
                    && (node_tmp.col() >= 0) && (node_tmp.col() < board_size_)) {
                bg_.filter_edges(node1, node_tmp);
                edges->push_back(std::make_pair(node1, node_tmp));
            }

            node_tmp = Node(wall.row() - 2, wall.col() + i);
            if ((node_tmp.row() >= 0) && (node_tmp.row() < board_size_)
                    && (node_tmp.col() >= 0) && (node_tmp.col() < board_size_)) {
                bg_.filter_edges(node_tmp, node2);
                edges->push_back(std::make_pair(node_tmp, node2));
            }

            for (int j = i - 1; j <= i + 1; j += 2) {
                node_tmp = Node(wall.row(), wall.col() + j);
                if ((node_tmp.row() >= 0) && (node_tmp.row() < board_size_)
                        && (node_tmp.col() >= 0) && (node_tmp.col() < board_size_)) {
                    bg_.filter_edges(node1, node_tmp);
                    edges->push_back(std::make_pair(node1, node_tmp));
                }

                node_tmp = Node(wall.row() - 1, wall.col() + j);
                if ((node_tmp.row() >= 0) && (node_tmp.row() < board_size_)
                        && (node_tmp.col() >= 0) && (node_tmp.col() < board_size_)) {
                    bg_.filter_edges(node_tmp, node2);
                    edges->push_back(std::make_pair(node_tmp, node2));
                }
            }
        }
    }
    else if (wall.orientation() == Wall::kVertical) {
        for (int i = 0; i < wall.cnt(); ++i) {
            node1 = Node(wall.row() + i, wall.col() - 1);
            node2 = Node(wall.row() + i, wall.col());
            bg_.filter_edges(node1, node2);
            edges->push_back(std::make_pair(node1, node2));

            node_tmp = Node(wall.row() + i, wall.col() + 1);
            if ((node_tmp.row() >= 0) && (node_tmp.row() < board_size_)
                    && (node_tmp.col() >= 0) && (node_tmp.col() < board_size_)) {
                bg_.filter_edges(node1, node_tmp);
                edges->push_back(std::make_pair(node1, node_tmp));
            }

            node_tmp = Node(wall.row() + i, wall.col() - 2);
            if ((node_tmp.row() >= 0) && (node_tmp.row() < board_size_)
                    && (node_tmp.col() >= 0) && (node_tmp.col() < board_size_)) {
                bg_.filter_edges(node_tmp, node2);
                edges->push_back(std::make_pair(node_tmp, node2));
            }

            for (int j = i - 1; j <= i + 1; j += 2) {
                node_tmp = Node(wall.row() + j, wall.col());
                if ((node_tmp.row() >= 0) && (node_tmp.row() < board_size_)
                        && (node_tmp.col() >= 0) && (node_tmp.col() < board_size_)) {
                    bg_.filter_edges(node1, node_tmp);
                    edges->push_back(std::make_pair(node1, node_tmp));
                }

                node_tmp = Node(wall.row() + j, wall.col() - 1);
                if ((node_tmp.row() >= 0) && (node_tmp.row() < board_size_)
                        && (node_tmp.col() >= 0) && (node_tmp.col() < board_size_)) {
                    bg_.filter_edges(node_tmp, node2);
                    edges->push_back(std::make_pair(node_tmp, node2));
                }
            }
        }
    }
    else {
        return -1;
    }

    bool path_blocked = false;

    for (auto pawn_data : pawn_data_list_) {
        path_blocked = true;
        for (auto goal_node : pawn_data.goal_nodes) {
            if (bg_.is_path_exists(pawn_data.node, goal_node)) {
                path_blocked = false;
                break;
            }
        }

        // wall blocks all pathes to the opposite side for one of pawns
        if (path_blocked) {
            break;
        }
    }

    if (path_blocked) {
        return -1;
    }

    return 0;
}

}  // namespace Quoridor
