#pragma once

template <typename T>
T max (T a, T b) { if (a > b) return a; return b; }

template <typename T>
T min (T a, T b) { if (a < b) return a; return b; }

template <typename T>
T max (T a, T b, T c) { if (a > b && a > c) return a; if (b > a && b > c) return b; return c; }

template <typename T>
T min (T a, T b, T c) { if (a < b && a < c) return a; if (b < a && b < c) return b; return c; }