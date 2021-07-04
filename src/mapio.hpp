#pragma once
#include "table.hpp"
#include <iostream>
#include <vector>


void inputmap(std::istream & in, table & map) {
    for (auto & row : map) {
        for (auto & e : row) {
            int16_t i = 0;
            in >> i;
            e = static_cast<int8_t>(i);
        }
    }
}


void outputmap(std::ostream & out, table & map) {
    for (auto & row : map) {
        for (auto & e : row) {
            out << static_cast<int16_t>(e) << ' ';
        }
        out << '\n';
    }
    out << std::flush;
}
