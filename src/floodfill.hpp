#pragma once
#include "point.hpp"
#include "table.hpp"
#include <stdexcept>
#include <vector>
#include <stack>
#include <queue>
#include <set>


size_t floodfill(table & map, const point start) {
    const size_t M = map.size();    if (M == 0) throw std::runtime_error("M should be greater than 0");
    const size_t N = map[0].size(); if (N == 0) throw std::runtime_error("N should be greater than 0");

    size_t visited = 0;
    if (!map[start.Y][start.X]) {
        return visited;
    }
    
    std::stack<point> frontier;
    
    frontier.emplace(start);
    map[start.Y][start.X] = false;
    ++visited;
    while (!frontier.empty()) {
        const auto c = frontier.top();
        frontier.pop();

        for (const auto & next : {
            point { c.X    , c.Y + 1 },
            point { c.X    , c.Y - 1 },
            point { c.X + 1, c.Y     },
            point { c.X - 1, c.Y     }
        }) {
            if(!isValid(next, N, M)) continue;
            if (map[next.Y][next.X]) {
                frontier.emplace(next);
                map[next.Y][next.X] = false;
                ++visited;
            }
        }
    }
    return visited;
}


std::set<point> floodfill_multithreaded(const table & map, const point start) {
    const size_t M = map.size();    if (M == 0) throw std::runtime_error("M should be greater than 0");
    const size_t N = map[0].size(); if (N == 0) throw std::runtime_error("N should be greater than 0");
    
    std::queue<point> frontier;
    frontier.emplace(start);
    std::set<point> reached;
    if (!map[start.Y][start.X]) {
        return reached;
    }
    reached.emplace(start);
    
    while (!frontier.empty()) {
        const point c = frontier.front();
        frontier.pop();
        for (const auto & next : {
            point { c.X    , c.Y + 1 },
            point { c.X    , c.Y - 1 },
            point { c.X + 1, c.Y     },
            point { c.X - 1, c.Y     }
        }) {
            if (reached.contains(next) || !isValid(next, N, M) || !map[next.Y][next.X]) continue;
            frontier.emplace(next);
            reached.emplace(next);
        }
        
    }
    return reached;
}
