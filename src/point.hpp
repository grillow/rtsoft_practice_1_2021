#pragma once


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
