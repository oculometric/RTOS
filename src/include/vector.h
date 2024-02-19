#pragma once

#include <stdint.h>

#include <math.h>

namespace nov
{
namespace vector
{

// represents a mathematical vector with 3 components
template<typename T>
struct nov_vector3
{
    T x, y, z;

    constexpr nov_vector3<T>(const nov_vector3<T>& o) : x(o.x), y(o.y), z(o.z) { }
    constexpr nov_vector3<T>(const T _x, const T _y, const T _z) : x(_x), y(_y), z(_z) { }
    constexpr nov_vector3<T>() : x(0), y(0), z(0) { }

    inline void operator=(const nov_vector3<T> & a) { this->x = a.x; this->y = a.y; this->z = a.z; }
    inline void operator+=(const nov_vector3<T> & a) { this->x += a.x; this->y += a.y; this->z += a.z; }
    inline void operator-=(const nov_vector3<T> & a) { this->x -= a.x; this->y -= a.y; this->z -= a.z; }
    inline void operator*=(const nov_vector3<T> & a) { this->x *= a.x; this->y *= a.y; this->z *= a.z; }
    inline void operator/=(const nov_vector3<T> & a) { this->x /= a.x; this->y /= a.y; this->z /= a.z; }
    inline void operator*=(const T f) { this->x *= f; this->y *= f; this->z *= f; }
    inline void operator/=(const T f) { this->x /= f; this->y /= f; this->z /= f; }
    inline nov_vector3<T> operator -() { return nov_vector3<T>{ -this->x, -this->y, -this->z }; }
};

template<typename T>
inline nov_vector3<T> operator+(const nov_vector3<T> & a, const nov_vector3<T> & b) { return nov_vector3<T>{ a.x+b.x, a.y+b.y, a.z+b.z }; }
template<typename T>
inline nov_vector3<T> operator-(const nov_vector3<T> & a, const nov_vector3<T> & b) { return nov_vector3<T>{ a.x-b.x, a.y-b.y, a.z-b.z }; }
template<typename T>
inline nov_vector3<T> operator*(const nov_vector3<T> & a, const nov_vector3<T> & b) { return nov_vector3<T>{ a.x*b.x, a.y*b.y, a.z*b.z }; }
template<typename T>
inline nov_vector3<T> operator/(const nov_vector3<T> & a, const nov_vector3<T> & b) { return nov_vector3<T>{ a.x/b.x, a.y/b.y, a.z/b.z }; }
template<typename T>
inline nov_vector3<T> operator*(const nov_vector3<T> & a, const T f) { return nov_vector3<T>{ a.x*f, a.y*f, a.z*f }; }
template<typename T>
inline nov_vector3<T> operator/(const nov_vector3<T> & a, const T f) { return nov_vector3<T>{ a.x/f, a.y/f, a.z/f }; }
template<typename T>
inline bool operator==(const nov_vector3<T> & a, const nov_vector3<T> & b) { return (a.x==b.x) && (a.y==b.y) && (a.z==b.z); }

// dot product between two vectors. this represents the magnitude of the two vectors multiplied together,
// multiplied by the cosine of the angle between them
template<typename T>
inline T operator^(const nov_vector3<T> & a, const nov_vector3<T> & b) { return (a.x*b.x)+(a.y*b.y)+(a.z*b.z); }
// cross product. this represents the magnitude of the two vectors multiplied together, 
// multiplied by the sine of the angle between them, multiplied by the vector which is normal to
// the two input vectors. when oriented with the two vectors pointing as upward as possible visually,
// left vector cross right vector will point directly into the camera
template<typename T>
inline nov_vector3<T> operator%(const nov_vector3<T> & a, const nov_vector3<T> & b) { return nov_vector3<T>{ (a.y*b.z)-(a.z*b.y), (a.z*b.x)-(a.x*b.z), (a.x*b.y)-(a.y*b.x) }; }

// calculate square magnitude of a vector
template<typename T>
inline T mag_sq(const nov_vector3<T> & a) { return (a.x*a.x)+(a.y*a.y)+(a.z*a.z); }
// calcualte magnitude of a vector
template<typename T>
inline T mag(const nov_vector3<T> & a) { return sqrt((a.x*a.x)+(a.y*a.y)+(a.z*a.z)); }

// normalise a vector in place
template<typename T>
inline void norm_self(nov_vector3<T> & a) { a /= mag(a); }
// calculate a normalised copy of a vector
template<typename T>
inline nov_vector3<T> norm_copy(const nov_vector3<T> & a) { return a / mag(a); }


// represents a 2 dimensional vector, with associated operations
template<typename T>
struct nov_vector2
{
    T u,v;

    constexpr nov_vector2<T>(const nov_vector2<T>& o) { this->u = o.u; this->v = o.v; }
    constexpr nov_vector2<T>(const T _u, const T _v) { this->u = _u; this->v = _v; }
    constexpr nov_vector2<T>() { this->u = 0; this->v = 0; }

    inline void operator=(const nov_vector2<T> & a) { this->u = a.u; this->v = a.v; }
    inline void operator+=(const nov_vector2<T> & a) { this->u += a.u; this->v += a.v; }
    inline void operator-=(const nov_vector2<T> & a) { this->u -= a.u; this->v -= a.v; }
    inline void operator*=(const nov_vector2<T> & a) { this->u *= a.u; this->v *= a.v; }
    inline void operator/=(const nov_vector2<T> & a) { this->u /= a.u; this->v /= a.v; }
    inline void operator*=(const T f) { this->u *= f; this->v *= f; }
    inline void operator/=(const T f) { this->u /= f; this->v /= f; }
    inline nov_vector2<T> operator -() { return nov_vector2<T>{ -this->u, -this->v }; }
};

template<typename T>
inline nov_vector2<T> operator+(const nov_vector2<T> & a, const nov_vector2<T> & b) { return nov_vector2<T>{ a.u+b.u, a.v+b.v }; }
template<typename T>
inline nov_vector2<T> operator-(const nov_vector2<T> & a, const nov_vector2<T> & b) { return nov_vector2<T>{ a.u-b.u, a.v-b.v }; }
template<typename T>
inline nov_vector2<T> operator*(const nov_vector2<T> & a, const nov_vector2<T> & b) { return nov_vector2<T>{ a.u*b.u, a.v*b.v }; }
template<typename T>
inline nov_vector2<T> operator/(const nov_vector2<T> & a, const nov_vector2<T> & b) { return nov_vector2<T>{ a.u/b.u, a.v/b.v }; }
template<typename T>
inline nov_vector2<T> operator*(const nov_vector2<T> & a, const T f) { return nov_vector2<T>{ a.u*f, a.v*f }; }
template<typename T>
inline nov_vector2<T> operator/(const nov_vector2<T> & a, const T f) { return nov_vector2<T>{ a.u/f, a.v/f }; }
template<typename T>
inline bool operator==(const nov_vector2<T> & a, const nov_vector2<T> & b) { return (a.u==b.u) && (a.v==b.v); }

// dot product between two vectors. this represents the magnitude of the two vectors multiplied together,
// multiplied by the cosine of the angle between them
template<typename T>
inline T operator^(const nov_vector2<T> & a, const nov_vector2<T> & b) { return (a.u*b.u)+(a.v*b.v); }

// calculate square magnitude of a vector
template<typename T>
inline T mag_sq(const nov_vector2<T> & a) { return (a.u*a.u)+(a.v*a.v); }
// calcualte magnitude of a vector
template<typename T>
inline T mag(const nov_vector2<T> & a) { return sqrt((a.u*a.u)+(a.v*a.v)); }

// normalise a vector in place
template<typename T>
inline void norm_self(nov_vector2<T> & a) { a /= mag(a); }
// calculate a normalised copy of a vector
template<typename T>
inline nov_vector3<T> norm_copy(const nov_vector2<T> & a) { return a / mag(a); }

// linear interpolate between two vectors
template<typename T>
inline nov_vector3<T> lerp(const nov_vector3<T> & a, const nov_vector3<T> & b, const T f) { nov_vector3<T>{ a.x+((b.x-a.x)*f), a.y+((b.y-a.y)*f), a.z+((b.z-a.z)*f) }; }
// linear interpolate between two vectors
template<typename T>
inline nov_vector2<T> lerp(const nov_vector2<T> & a, const nov_vector2<T> & b, const T f) { nov_vector2<T>{ a.u+((b.u-a.u)*f), a.v+((b.v-a.v)*f) }; }
// reflect v in w, where w can be interpreted as a vector normal to a mirror in which v is reflected
template<typename T>
inline nov_vector3<T> reflect(const nov_vector3<T> & a, const nov_vector3<T> & b) { return a-(b*(a^b)*2); }

typedef nov_vector3<uint8_t> nov_colour;
typedef nov_vector3<float> nov_fvector3;
typedef nov_vector3<uint32_t> nov_uvector3;
typedef nov_vector3<int32_t> nov_ivector3;

typedef nov_vector2<float> nov_fvector2;
typedef nov_vector2<uint32_t> nov_uvector2;
typedef nov_vector2<int32_t> nov_ivector2;

}
}