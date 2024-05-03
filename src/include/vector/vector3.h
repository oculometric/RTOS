#pragma once

#include <math.h>

namespace nov
{
namespace vector
{

// represents a mathematical vector with 3 components
template<typename T>
struct Vector3
{
    T x, y, z;

    constexpr Vector3<T>(const Vector3<T>& o) : x(o.x), y(o.y), z(o.z) { }
    constexpr Vector3<T>(const T _x, const T _y, const T _z) : x(_x), y(_y), z(_z) { }
    constexpr Vector3<T>() : x(0), y(0), z(0) { }

    inline void operator=(const Vector3<T>& a) { x = a.x; y = a.y; z = a.z; }
    inline void operator+=(const Vector3<T>& a) { x += a.x; y += a.y; z += a.z; }
    inline void operator-=(const Vector3<T>& a) { x -= a.x; y -= a.y; z -= a.z; }
    inline void operator*=(const Vector3<T>& a) { x *= a.x; y *= a.y; z *= a.z; }
    inline void operator/=(const Vector3<T>& a) { x /= a.x; y /= a.y; z /= a.z; }
    inline void operator*=(const T f) { x *= f; y *= f; z *= f; }
    inline void operator/=(const T f) { x /= f; y /= f; z /= f; }
    inline Vector3<T> operator -() { return Vector3<T>{ -x, -y, -z }; }
};

template<typename T>
inline Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b) { return Vector3<T>{ a.x+b.x, a.y+b.y, a.z+b.z }; }
template<typename T>
inline Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b) { return Vector3<T>{ a.x-b.x, a.y-b.y, a.z-b.z }; }
template<typename T>
inline Vector3<T> operator*(const Vector3<T>& a, const Vector3<T>& b) { return Vector3<T>{ a.x*b.x, a.y*b.y, a.z*b.z }; }
template<typename T>
inline Vector3<T> operator/(const Vector3<T>& a, const Vector3<T>& b) { return Vector3<T>{ a.x/b.x, a.y/b.y, a.z/b.z }; }
template<typename T>
inline Vector3<T> operator*(const Vector3<T>& a, const T f) { return Vector3<T>{ a.x*f, a.y*f, a.z*f }; }
template<typename T>
inline Vector3<T> operator/(const Vector3<T>& a, const T f) { return Vector3<T>{ a.x/f, a.y/f, a.z/f }; }
template<typename T>
inline bool operator==(const Vector3<T>& a, const Vector3<T>& b) { return (a.x==b.x) && (a.y==b.y) && (a.z==b.z); }

// dot product between two vectors. this represents the magnitude of the two vectors multiplied together,
// multiplied by the cosine of the angle between them
template<typename T>
inline T operator^(const Vector3<T>& a, const Vector3<T>& b) { return (a.x*b.x)+(a.y*b.y)+(a.z*b.z); }
// cross product. this represents the magnitude of the two vectors multiplied together, 
// multiplied by the sine of the angle between them, multiplied by the vector which is normal to
// the two input vectors. when oriented with the two vectors pointing as upward as possible visually,
// left vector cross right vector will point directly into the camera
template<typename T>
inline Vector3<T> operator%(const Vector3<T>& a, const Vector3<T>& b) { return Vector3<T>{ (a.y*b.z)-(a.z*b.y), (a.z*b.x)-(a.x*b.z), (a.x*b.y)-(a.y*b.x) }; }

// calculate square magnitude of a vector
template<typename T>
inline T magSq(const Vector3<T>& a) { return (a.x*a.x)+(a.y*a.y)+(a.z*a.z); }
// calcualte magnitude of a vector
template<typename T>
inline T mag(const Vector3<T>& a) { return sqrt(magSq(a)); }

// calculate a normalised copy of a vector
template<typename T>
inline Vector3<T> norm(const Vector3<T>& a) { return a / mag(a); }

// linear interpolate between two vectors
template<typename T>
inline Vector3<T> lerp(const Vector3<T>& a, const Vector3<T>& b, const T f) { Vector3<T>{ a.x+((b.x-a.x)*f), a.y+((b.y-a.y)*f), a.z+((b.z-a.z)*f) }; }

// reflect v in w, where w can be interpreted as a vector normal to a mirror in which v is reflected
template<typename T>
inline Vector3<T> reflect(const Vector3<T>& a, const Vector3<T>& b) { return a-(b*(a^b)*2); }

}
}