#ifndef QUORIDOR_GAME_HPP_
#define QUORIDOR_GAME_HPP_

#include <map>
#include <memory>
#include <set>
#include <vector>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/tag.hpp>

#include "board_graph.hpp"
#include "node.hpp"
#include "pawn.hpp"
#include "walk_move.hpp"
#include "wall_grid.hpp"
#include "wall_move.hpp"

namespace Quoridor {

struct pawn_data_t {
    int idx;
    std::shared_ptr<Pawn> pawn;
    Node node;
    std::set<Node> goal_nodes;
};

struct by_pawn {};

typedef boost::multi_index_container<
    pawn_data_t,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_unique<boost::multi_index::member<
            pawn_data_t, int, &pawn_data_t::idx>>,
        boost::multi_index::ordered_unique<
            boost::multi_index::tag<by_pawn>,
            boost::multi_index::member<
                pawn_data_t, std::shared_ptr<Pawn>, &pawn_data_t::pawn>>
    >
> pawn_data_list_t;

class Game {
public:
    explicit Game(int board_size);
    virtual ~Game();

    void set_pawns(std::vector<std::shared_ptr<Pawn>> &pawn_list);
    void switch_pawn();
    const pawn_data_t &cur_pawn_data() const;
    const pawn_data_list_t &pawn_data_list() const;
    const pawn_data_t &pawn_data(std::shared_ptr<Pawn> &pawn) const;

    int move_pawn(const Node &node);
    int add_wall(const Wall &wall);

    bool is_finished() const;

    size_t shortest_path(const Node &start_node,
            const std::set<Node> &goal_nodes, std::list<Node> *path) const;
    bool get_path(std::shared_ptr<Pawn> pawn, const Node &node,
            std::list<Node> *path) const;

    void possible_moves(std::shared_ptr<Pawn> pawn, std::vector<IMove*> *moves)
        const;

private:
    int try_add_wall(const Wall &wall,
            std::vector<std::pair<Node, Node>> *edges);

private:
    int board_size_;
    pawn_data_list_t pawn_data_list_;
    int cur_pawn_idx_;
    BoardGraph bg_;
    WallGrid wg_;
};

}  // namespace Quoridor

#endif  // QUORIDOR_GAME_HPP_
