#ifndef QUORIDOR_WALL_GRID_HPP_
#define QUORIDOR_WALL_GRID_HPP_

#include <map>
#include <vector>
#include "node.hpp"
#include "wall.hpp"

namespace Quoridor {

class WallGrid {
public:
    explicit WallGrid(int size);
    virtual ~WallGrid();

    int add_wall(const Wall &wall);
    int add_tmp_wall(const Wall &wall) const;
    void apply_tmp_wall();

    void possible_walls(std::vector<Wall> *walls) const;

private:
    std::map<Node, int> occupied_nodes_;
    mutable std::map<Node, int> tmp_occupied_nodes_;
    int size_;
};

}  // namespace Quoridor

#endif  // QUORIDOR_WALL_GRID_HPP_
