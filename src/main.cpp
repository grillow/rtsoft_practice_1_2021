#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>


void inputmap(std::istream & in, std::vector<std::vector<int8_t>> & map) {
    for (auto & row : map) {
        for (auto & e : row) {
            int16_t i = 0;
            in >> i;
            e = static_cast<int8_t>(i);
        }
    }
}


void outputmap(std::ostream & out, std::vector<std::vector<int8_t>> & map) {
    for (auto & row : map) {
        for (auto & e : row) {
            out << static_cast<int16_t>(e) << ' ';
        }
        out << '\n';
    }
    out << std::flush;
}


struct point {
    int X;
    int Y;
    friend constexpr inline bool operator< (const auto & l, const auto & r) {
        return l.X < r.X || (l.X == r.X && l.Y < r.Y );
    }
};


constexpr inline bool isValid(const point & pos, const int borderX, const int borderY) {
    return (pos.X >= 0 && pos.Y >= 0 && pos.X < borderX && pos.Y < borderY);
}


// I am forced to use int8_t instead of bool since stl decided to break std::vector<bool>
size_t floodfill(std::vector<std::vector<int8_t>> & map, const point start) {
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


std::set<point> floodfill_multithreaded(const std::vector<std::vector<int8_t>> & map, const point start) {
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


int main() {
    size_t N = 0;
    std::cin >> N;
    size_t M = 0;
    std::cin >> M;
    
    std::vector<std::vector<int8_t>> map(M, std::vector<int8_t>(N, false));
    inputmap(std::cin, map);
    outputmap(std::cout, map);
    
    if (false) {    // efficient single-threaded
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
        std::cout << max_square << std::endl;
        //outputmap(std::cout, map);
    } else {    // multi-threaded
        const size_t total = N * M;
        size_t max_square = 0;
        //std::set<point> max_island;
        //point max_start = {0,0};
        size_t current_position = 0;
        std::set<point> unreached;
        for (size_t i = 0; i < N * M; ++i) {
            unreached.emplace(point { static_cast<int>(i % N), static_cast<int>(i / N) });
        }
        while (current_position != total) {
            const int X = current_position % N;
            const int Y = current_position / N;
            if (unreached.contains(point { X, Y })) {
                const auto island = floodfill_multithreaded(map, point { X, Y });
                max_square = std::max(max_square, island.size());
                //if (island.size() > max_square) {
                    //max_square = island.size();
                    //max_island = island;
                    //max_start = {X, Y};
                //}
                std::erase_if(unreached, [&](const auto & e)->auto { return island.contains(e); });
            }
            ++current_position;
        }
        std::cout << max_square << std::endl;
        //std::cout << max_square << " " << max_start.X << " " << max_start.Y << std::endl;
        //std::for_each(max_island.begin(), max_island.end(), [](const auto & e) { std::cout << e.X << " " << e.Y << std::endl; });
        //outputmap(std::cout, map);
    }

    return 0;
}
