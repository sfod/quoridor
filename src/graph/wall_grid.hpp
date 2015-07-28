#pragma once

#include <map>
#include <vector>
#include "node.hpp"
#include "wall.hpp"

class WallGrid {
public:
    explicit WallGrid(int size);

    int add_wall(const Wall &wall);
    bool add_tmp_wall(const Wall &wall) const;
    void apply_tmp_wall();

    void possible_walls(std::vector<Wall> *walls) const;

private:
    std::map<Node, int> occupied_nodes_;
    mutable std::map<Node, int> tmp_occupied_nodes_;
    int size_;
};
