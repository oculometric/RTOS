#pragma once

template <typename T>
inline T max(T a, T b) { if (a > b) return a; return b; }

template <typename T>
inline T min(T a, T b) { if (a < b) return a; return b; }

template <typename T>
inline T max(T a, T b, T c) { if (a > b && a > c) return a; if (b > a && b > c) return b; return c; }

template <typename T>
inline T min(T a, T b, T c) { if (a < b && a < c) return a; if (b < a && b < c) return b; return c; }

template <typename T>
inline T abs(T a) { return a < 0 ? -a : a; }

// TODO: move these functions to calling assembly instructions
inline float inv_sqrt(float number)
{ // literally just the quake iii code
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
    // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}

inline float sqrt(float number) { return 1.0f/inv_sqrt(number); }

inline float modf(float a, float b)
{
    float f = a;
    while (f > b) f -= b;
    return f;
}