#pragma once
#include "point.hpp"
#include "mapio.hpp"
#include "floodfill.hpp"

#include <algorithm>
#include <ranges>
#include <thread>
#include <future>


// worker function
size_t worker(const std::vector<std::vector<int8_t>> & map, const size_t begin, const size_t end) {
    const size_t M = map.size();    if (M == 0) throw std::runtime_error("M should be greater than 0");
    const size_t N = map[0].size(); if (N == 0) throw std::runtime_error("N should be greater than 0");
    
    size_t max_square = 0;

    size_t current_position = begin;

    std::set<point> unreached;
    for (size_t i = begin; i < end; ++i) {
        unreached.emplace(point { static_cast<int>(i % N), static_cast<int>(i / N) });
    }

    while (current_position < end) {
        const int X = current_position % N;
        const int Y = current_position / N;
        
        if (unreached.contains(point { X, Y })) {
            const auto island = floodfill_multithreaded(map, point { X, Y });
            max_square = std::max(max_square, island.size());
            std::erase_if(unreached, [&](const auto & e)->auto { return island.contains(e); });
        }

        ++current_position;
    }

    return max_square;
}


// multi-threaded
size_t solve_multi(std::vector<std::vector<int8_t>> map, const size_t CPU) {
    const size_t M = map.size();    if (M == 0) throw std::runtime_error("M should be greater than 0");
    const size_t N = map[0].size(); if (N == 0) throw std::runtime_error("N should be greater than 0");
        
    const size_t total = N * M;
    const size_t piece_size = total / CPU;
    
    std::vector<std::future<size_t>> max_squares(CPU);

    std::generate(max_squares.begin(), max_squares.end(),
        [&, i = 0u](){
            const size_t begin = i * piece_size;
            const size_t end = (i + 1) * piece_size + ( (i == CPU - 1) * (total - piece_size * CPU) );   // tail goes to the last thread
            return std::async(worker, std::ref(map), begin, end);
        }
    );

    /*
    std::vector<std::thread> workers; workers.reserve(CPU);
    for (size_t i = 0; i < CPU; ++i) {
        const size_t total = N * M;
        const size_t piece_size = total / CPU;
        const size_t begin = i * piece_size;
        const size_t end = (i + 1) * piece_size + ( (i == CPU -1) * (total - piece_size * CPU) );   // tail goes to the last thread
        workers.emplace_back(worker_f, std::ref(map), begin, end, std::ref(max_squares[i]));
    }
    for (auto & worker : workers) {
        worker.join();
    }
    */

    const size_t max_square = std::max(max_squares | std::ranges::views::transform( [](auto & future){ return future.get(); } ));
    return max_square;
}
