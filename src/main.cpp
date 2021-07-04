#include <iostream>

#include "mapio.hpp"
#include "solve_single.hpp"
#include "solve_multi.hpp"


int main() {
    size_t N = 0;
    std::cin >> N;
    size_t M = 0;
    std::cin >> M;
    
    // I am forced to use int8_t instead of bool since stl decided to break std::vector<bool>
    std::vector<std::vector<int8_t>> map(M, std::vector<int8_t>(N, false));
    inputmap(std::cin, map);
    //outputmap(std::cout, map);
   
    const size_t max_square = solve_multi(map, std::thread::hardware_concurrency());

    std::cout << max_square << std::endl;

    return 0;
}
