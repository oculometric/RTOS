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

template <typename T>
inline T sign(T a) { return a > 0 ? 1 : -1; }

inline float sqrt(float number)
{
    float f;
    asm("fsqrt" : "=t" (f) : "0" (number));

    return f;
}

inline float invSqrt(float number)
{ 
    return 1.0f/sqrt(number);
}

inline float modf(float a, float b)
{
    float f = a;
    while (f > b) f -= b;
    return f;
}

inline float tanf(float rad)
{
    float f;
    asm("fptan;"
    "fdivp" : "=t" (f) : "0" (rad));
    return f;
}

inline float sinf(float rad)
{
    float f;
    asm("fsin" : "=t" (f) : "0" (rad));
    return f;
}

inline float cosf(float rad)
{
    float f;
    asm("fcos" : "=t" (f) : "0" (rad));
    return f;
}

#define MATH_PI 3.14159265359