#pragma once

#include <math.h>

namespace nov
{
namespace vector
{

// represents a mathematical vector with 3 components
template<typename T>
struct nov_vector4
{
    T x, y, z, w;

    constexpr nov_vector4<T>(const nov_vector4<T>& o) : x(o.x), y(o.y), z(o.z), w(o.w) { }
    constexpr nov_vector4<T>(const nov_vector3<T>& o) : x(o.x), y(o.y), z(o.z), w(1) { }
    constexpr nov_vector4<T>(const T _x, const T _y, const T _z, const T _w) : x(_x), y(_y), z(_z), w(_w) { }
    constexpr nov_vector4<T>() : x(0), y(0), z(0), w(0) { }

    inline void operator=(const nov_vector4<T>& a) { x = a.x; y = a.y; z = a.z; w = a.w; }
    inline void operator+=(const nov_vector4<T>& a) { x += a.x; y += a.y; z += a.z; w += a.w; }
    inline void operator-=(const nov_vector4<T>& a) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; }
    inline void operator*=(const nov_vector4<T>& a) { x *= a.x; y *= a.y; z *= a.z; w *= a.w; }
    inline void operator/=(const nov_vector4<T>& a) { x /= a.x; y /= a.y; z /= a.z; w /= a.w; }
    inline void operator*=(const T f) { x *= f; y *= f; z *= f; w *= f; }
    inline void operator/=(const T f) { x /= f; y /= f; z /= f; w /= f; }
    inline nov_vector4<T> operator -() { return nov_vector4<T>{ -x, -y, -z, -w }; }
};

template<typename T>
inline nov_vector4<T> operator+(const nov_vector4<T>& a, const nov_vector4<T>& b) { return nov_vector4<T>{ a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w }; }
template<typename T>
inline nov_vector4<T> operator-(const nov_vector4<T>& a, const nov_vector4<T>& b) { return nov_vector4<T>{ a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w }; }
template<typename T>
inline nov_vector4<T> operator*(const nov_vector4<T>& a, const nov_vector4<T>& b) { return nov_vector4<T>{ a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w }; }
template<typename T>
inline nov_vector4<T> operator/(const nov_vector4<T>& a, const nov_vector4<T>& b) { return nov_vector4<T>{ a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w }; }
template<typename T>
inline nov_vector4<T> operator*(const nov_vector4<T>& a, const T f) { return nov_vector4<T>{ a.x*f, a.y*f, a.z*f, a.w*f }; }
template<typename T>
inline nov_vector4<T> operator/(const nov_vector4<T>& a, const T f) { return nov_vector4<T>{ a.x/f, a.y/f, a.z/f, a.w/f }; }
template<typename T>
inline bool operator==(const nov_vector4<T>& a, const nov_vector4<T>& b) { return (a.x==b.x) && (a.y==b.y) && (a.z==b.z); && (a.w==b.w); }

// dot product between two vectors. this represents the magnitude of the two vectors multiplied together,
// multiplied by the cosine of the angle between them
template<typename T>
inline T operator^(const nov_vector4<T>& a, const nov_vector4<T>& b) { return (a.x*b.x)+(a.y*b.y)+(a.z*b.z)+(a.w*b.w); }
// calculate square magnitude of a vector
template<typename T>
inline T mag_sq(const nov_vector4<T>& a) { return (a.x*a.x)+(a.y*a.y)+(a.z*a.z)+(a.w*a.w); }
// calcualte magnitude of a vector
template<typename T>
inline T mag(const nov_vector4<T>& a) { return sqrt((a.x*a.x)+(a.y*a.y)+(a.z*a.z)+(a.w*a.w)); }

// calculate a normalised copy of a vector
template<typename T>
inline nov_vector4<T> norm(const nov_vector4<T>& a) { return a / mag(a); }

// linear interpolate between two vectors
template<typename T>
inline nov_vector4<T> lerp(const nov_vector4<T>& a, const nov_vector4<T>& b, const T f) { nov_vector4<T>{ a.x+((b.x-a.x)*f), a.y+((b.y-a.y)*f), a.z+((b.z-a.z)*f), a.w+((b.w-a.w)*f) }; }

}
}