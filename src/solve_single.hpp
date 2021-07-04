#pragma once
#include "floodfill.hpp"


// efficient single-threaded
size_t solve_single(table map) {
    const size_t M = map.size();    if (M == 0) throw std::runtime_error("M should be greater than 0");
    const size_t N = map[0].size(); if (N == 0) throw std::runtime_error("N should be greater than 0");
    
    const size_t total = N * M;
    size_t max_square = 0;
    size_t current_position = 0;
    while (current_position != total) {
        const size_t X = current_position % N;
        const size_t Y = current_position / N;
        const auto island = floodfill(map, point { static_cast<int>(X), static_cast<int>(Y) });
        max_square = std::max(max_square, island);
        ++current_position;
    }
    return max_square;
}
