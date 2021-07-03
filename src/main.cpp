#include <iostream>
#include <vector>
#include <queue>
#include <stack>


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
};


constexpr inline bool isValid(const point & pos, const int borderX, const int borderY) {
    return (pos.X >= 0 && pos.Y >= 0 && pos.X < borderX && pos.Y < borderY);
}


// I am forced to use int8_t instead of bool since stl decided to break std::vector<bool>
std::vector<point> floodfill(std::vector<std::vector<int8_t>> map, const point start) {
    const size_t M = map.size();    if (M == 0) throw std::runtime_error("M should be greater than 0");
    const size_t N = map[0].size(); if (N == 0) throw std::runtime_error("N should be greater than 0");

    std::vector<point> visited;
    if (!map[start.X][start.Y]) {
        return visited;
    }
    
    std::stack<point> frontier;
    
    frontier.emplace(start);
    map[start.X][start.Y] = false;
    visited.emplace_back(start);
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
            if (map[next.X][next.Y]) {
                frontier.emplace(next);
                map[next.X][next.Y] = false;
                visited.emplace_back(next);
            }
        }
    }
    return visited;
}


int main() {
    size_t N = 0;
    std::cin >> N;
    size_t M = 0;
    std::cin >> M;
    
    std::vector<std::vector<int8_t>> map(N, std::vector<int8_t>(M, false));
    inputmap(std::cin, map);
    //outputmap(std::cout, map);
    
    const size_t total = N * M;
    size_t max_square = 0;
    size_t current_position = 0;
    while (current_position != total) {
        const size_t X = current_position % N;
        const size_t Y = current_position / N;
        const auto island = floodfill(map, point { static_cast<int>(X), static_cast<int>(Y) });
        max_square = std::max(max_square, island.size());
        for (const auto & dot : island) {
            map[dot.X][dot.Y] = false;
        }
        ++current_position;
    }
    std::cout << max_square << std::endl;
    return 0;
}
