#pragma once

#include "math.h"

template <typename T>
struct vector2
{
    T x;
    T y;

    inline vector2 operator*(T i) { return vector2{ x * i, y * i }; }
    inline vector2 operator/(T i) { return vector2{ x / i, y / i }; }

    inline vector2 operator/(vector2 b) { return vector2{ x / b.x, y / b.y }; }
    inline vector2 operator+(vector2 b) { return vector2{ x + b.x, y + b.y }; }
    inline vector2 operator*(vector2 b) { return vector2{ x * b.x, y * b.y }; }
    inline vector2 operator-(vector2 b) { return vector2{ x - b.x, y - b.y }; }
    inline vector2 operator%(vector2 b) { return vector2{ x % b.x, y % b.y }; }

};

inline vector2<int> abs(vector2<int> a) { return vector2<int>{ abs(a.x), abs(a.y) }; }
inline int magnitude_squared(vector2<int> a) { return (a.x * a.x) + (a.y * a.y); }
inline vector2<int> clamp(vector2<int> c, vector2<int> vmax, vector2<int> vmin) { return vector2<int>{ min(max(c.x, vmin.x), vmax.x), min(max(c.y, vmin.y), vmax.y) }; }