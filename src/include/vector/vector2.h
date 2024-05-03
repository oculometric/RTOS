#pragma once

#include <stdint.h>

namespace nov
{
namespace vector
{

// represents a 2 dimensional vector, with associated operations
template<typename T>
struct Vector2
{
    T u,v;

    constexpr Vector2<T>(const Vector2<T>& o) : u(o.u), v(o.v) { }
    constexpr Vector2<T>(const T _u, const T _v) : u(_u), v(_v) { }
    constexpr Vector2<T>(): u(0), v(0) { }

    inline void operator=(const Vector2<T>& a) { u = a.u; v = a.v; }
    inline void operator+=(const Vector2<T>& a) { u += a.u; v += a.v; }
    inline void operator-=(const Vector2<T>& a) { u -= a.u; v -= a.v; }
    inline void operator*=(const Vector2<T>& a) { u *= a.u; v *= a.v; }
    inline void operator/=(const Vector2<T>& a) { u /= a.u; v /= a.v; }
    inline void operator*=(const T f) { u *= f; v *= f; }
    inline void operator/=(const T f) { u /= f; v /= f; }
    inline Vector2<T> operator-() { return Vector2<T>{ -u, -v }; }
};

template<typename T>
inline Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b) { return Vector2<T>{ a.u+b.u, a.v+b.v }; }
template<typename T>
inline Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b) { return Vector2<T>{ a.u-b.u, a.v-b.v }; }
template<typename T>
inline Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b) { return Vector2<T>{ a.u*b.u, a.v*b.v }; }
template<typename T>
inline Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b) { return Vector2<T>{ a.u/b.u, a.v/b.v }; }
template<typename T>
inline Vector2<T> operator*(const Vector2<T>& a, const T f) { return Vector2<T>{ a.u*f, a.v*f }; }
template<typename T>
inline Vector2<T> operator/(const Vector2<T>& a, const T f) { return Vector2<T>{ a.u/f, a.v/f }; }
template<typename T>
inline bool operator==(const Vector2<T>& a, const Vector2<T>& b) { return (a.u==b.u) && (a.v==b.v); }

// dot product between two vectors. this represents the magnitude of the two vectors multiplied together,
// multiplied by the cosine of the angle between them
template<typename T>
inline T operator^(const Vector2<T>& a, const Vector2<T>& b) { return (a.u*b.u)+(a.v*b.v); }

// calculate square magnitude of a vector
template<typename T>
inline T magSq(const Vector2<T>& a) { return (a.u*a.u)+(a.v*a.v); }
// calcualte magnitude of a vector
template<typename T>
inline T mag(const Vector2<T>& a) { return sqrt((a.u*a.u)+(a.v*a.v)); }

// calculate a normalised version of a vector
template<typename T>
inline Vector2<T> norm(const Vector2<T>& a) { return a / mag(a); }

// linear interpolate between two vectors
template<typename T>
inline Vector2<T> lerp(const Vector2<T>& a, const Vector2<T>& b, const T f) { Vector2<T>{ a.u+((b.u-a.u)*f), a.v+((b.v-a.v)*f) }; }

}
}