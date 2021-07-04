#include <algorithm>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <set>

#include <thread>
#include <future>


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


// efficient single-threaded
size_t solve_single(std::vector<std::vector<int8_t>> map) {
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


// worker function
void worker_f(const std::vector<std::vector<int8_t>> & map, const size_t begin, const size_t end, size_t & max_square) {
    const size_t M = map.size();    if (M == 0) throw std::runtime_error("M should be greater than 0");
    const size_t N = map[0].size(); if (N == 0) throw std::runtime_error("N should be greater than 0");
    
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
}


// multi-threaded
size_t solve_multi(std::vector<std::vector<int8_t>> map, const size_t CPU) {
    const size_t M = map.size();    if (M == 0) throw std::runtime_error("M should be greater than 0");
    const size_t N = map[0].size(); if (N == 0) throw std::runtime_error("N should be greater than 0");
    
    std::vector<size_t> max_squares(CPU, 0);

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
    
    const size_t max_square = *std::max_element(max_squares.begin(), max_squares.end());
    return max_square;
}


int main() {
    size_t N = 0;
    std::cin >> N;
    size_t M = 0;
    std::cin >> M;
    
    std::vector<std::vector<int8_t>> map(M, std::vector<int8_t>(N, false));
    inputmap(std::cin, map);
    //outputmap(std::cout, map);
   
    const size_t max_square = solve_multi(map, std::thread::hardware_concurrency());

    std::cout << max_square << std::endl;

    return 0;
}
