#pragma once

#include <stdint.h>

#include "render_geometry.h"

inline int abs(int i) { return i >= 0 ? i : -i; }
inline int max(int a, int b) { return a > b ? a : b; }
inline int min(int a, int b) { return a < b ? a : b; }

inline float abs(float i) { return i >= 0 ? i : -i; }
inline float max(float a, float b) { return a > b ? a : b; }
inline float min(float a, float b) { return a < b ? a : b; }

inline int pow(int i, int p)
{
    int m = 1;
    for (int c = 0; c < p; c++) m *= i;
    return m;
}

inline float clamp0(float f) { return f < 0 ? 0 : f; }

inline void gamma_correct(const rtos_vector3& v, float exp, float gam, uint8_t* o)
{
    rtos_vector3 g = v * exp;
    g = rtos_vector3{ powf(clamp0(g.x),gam), powf(clamp0(g.y),gam), powf(clamp0(g.z),gam) };
    o[0] = (uint8_t)(g.x*UINT8_MAX);
    o[1] = (uint8_t)(g.y*UINT8_MAX);
    o[2] = (uint8_t)(g.z*UINT8_MAX);
}