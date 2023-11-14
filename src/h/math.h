#pragma once

inline int abs(int i) { return i >= 0 ? i : -i; }
inline int max(int a, int b) { return a > b ? a : b; }
inline int min(int a, int b) { return a < b ? a : b; }

inline float abs(float i) { return i >= 0 ? i : -i; }
inline float max(float a, float b) { return a > b ? a : b; }
inline float min(float a, float b) { return a < b ? a : b; }