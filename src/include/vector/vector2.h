#pragma once

#include <stdint.h>

namespace nov
{
namespace vector
{

// represents a 2 dimensional vector, with associated operations
template<typename T>
struct nov_vector2
{
    T u,v;

    constexpr nov_vector2<T>(const nov_vector2<T>& o) : u(o.u), v(o.v) { }
    constexpr nov_vector2<T>(const T _u, const T _v) : u(_u), v(_v) { }
    constexpr nov_vector2<T>(): u(0), v(0) { }

    inline void operator=(const nov_vector2<T>& a) { u = a.u; v = a.v; }
    inline void operator+=(const nov_vector2<T>& a) { u += a.u; v += a.v; }
    inline void operator-=(const nov_vector2<T>& a) { u -= a.u; v -= a.v; }
    inline void operator*=(const nov_vector2<T>& a) { u *= a.u; v *= a.v; }
    inline void operator/=(const nov_vector2<T>& a) { u /= a.u; v /= a.v; }
    inline void operator*=(const T f) { u *= f; v *= f; }
    inline void operator/=(const T f) { u /= f; v /= f; }
    inline nov_vector2<T> operator-() { return nov_vector2<T>{ -u, -v }; }
};

template<typename T>
inline nov_vector2<T> operator+(const nov_vector2<T>& a, const nov_vector2<T>& b) { return nov_vector2<T>{ a.u+b.u, a.v+b.v }; }
template<typename T>
inline nov_vector2<T> operator-(const nov_vector2<T>& a, const nov_vector2<T>& b) { return nov_vector2<T>{ a.u-b.u, a.v-b.v }; }
template<typename T>
inline nov_vector2<T> operator*(const nov_vector2<T>& a, const nov_vector2<T>& b) { return nov_vector2<T>{ a.u*b.u, a.v*b.v }; }
template<typename T>
inline nov_vector2<T> operator/(const nov_vector2<T>& a, const nov_vector2<T>& b) { return nov_vector2<T>{ a.u/b.u, a.v/b.v }; }
template<typename T>
inline nov_vector2<T> operator*(const nov_vector2<T>& a, const T f) { return nov_vector2<T>{ a.u*f, a.v*f }; }
template<typename T>
inline nov_vector2<T> operator/(const nov_vector2<T>& a, const T f) { return nov_vector2<T>{ a.u/f, a.v/f }; }
template<typename T>
inline bool operator==(const nov_vector2<T>& a, const nov_vector2<T>& b) { return (a.u==b.u) && (a.v==b.v); }

// dot product between two vectors. this represents the magnitude of the two vectors multiplied together,
// multiplied by the cosine of the angle between them
template<typename T>
inline T operator^(const nov_vector2<T>& a, const nov_vector2<T>& b) { return (a.u*b.u)+(a.v*b.v); }

// calculate square magnitude of a vector
template<typename T>
inline T mag_sq(const nov_vector2<T>& a) { return (a.u*a.u)+(a.v*a.v); }
// calcualte magnitude of a vector
template<typename T>
inline T mag(const nov_vector2<T>& a) { return sqrt((a.u*a.u)+(a.v*a.v)); }

// calculate a normalised version of a vector
template<typename T>
inline nov_vector2<T> norm(const nov_vector2<T>& a) { return a / mag(a); }

// linear interpolate between two vectors
template<typename T>
inline nov_vector2<T> lerp(const nov_vector2<T>& a, const nov_vector2<T>& b, const T f) { nov_vector2<T>{ a.u+((b.u-a.u)*f), a.v+((b.v-a.v)*f) }; }

}
}