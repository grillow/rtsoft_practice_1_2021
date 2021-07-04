#pragma once
#include <iostream>
#include <vector>


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
