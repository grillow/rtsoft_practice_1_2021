#include <iostream>
#include <vector>
#include <queue>
#include <stack>


void inputmap(std::istream & in, std::vector<std::vector<int8_t>> & map) {
    for (auto & row : map) {
        for (auto & e : row) {
            int i = 0;
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


struct pos {
    int i;
    int j;
};


constexpr inline bool isValid(const pos & point, const int borderX, const int borderY) {
        return (point.i >= 0 && point.j >= 0 && point.i < borderX && point.j < borderY);
}


// I am forced to use int8_t instead of bool since stl decided to break std::vector<bool>
std::vector<pos> floodfill(std::vector<std::vector<int8_t>> map, const pos start) {
    const size_t M = map.size();    if (M == 0) throw std::runtime_error("M should be greater than 0");
    const size_t N = map[0].size(); if (N == 0) throw std::runtime_error("N should be greater than 0");

    std::vector<pos> visited;
    if (!map[start.j][start.i]) {
        return visited;
    }
    
    std::stack<pos> frontier;
    
    frontier.emplace(start);
    map[start.j][start.i] = false;
    visited.emplace_back(start);
    while (!frontier.empty()) {
        const auto c = frontier.top();
        frontier.pop();

        for (const auto & next : {
            pos { c.i    , c.j + 1 },
            pos { c.i    , c.j - 1 },
            pos { c.i + 1, c.j     },
            pos { c.i - 1, c.j     }
        }) {
            if(!isValid(next, N, M)) continue;
            if (map[next.j][next.i]) {
                frontier.emplace(next);
                map[next.j][next.i] = false;
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
    
    std::vector<std::vector<int8_t>> map(M, std::vector<int8_t>(N, false));
    inputmap(std::cin, map);
    //outputmap(std::cout, map);
    
    const size_t total = N * M;
    size_t max_square = 0;
    size_t current_position = 0;
    while (current_position != total) {
        const size_t X = current_position % N;
        const size_t Y = current_position / N;
        const auto island = floodfill(map, pos { static_cast<int>(X), static_cast<int>(Y) });
        max_square = std::max(max_square, island.size());
        for (const auto & dot : island) {
            map[dot.j][dot.i] = false;
        }
        ++current_position;
    }
    std::cout << max_square << std::endl;
    return 0;
}
