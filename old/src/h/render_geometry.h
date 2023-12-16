#include "math.h"

struct rtos_vector3
{
    float x, y, z;

    inline void operator=(const rtos_vector3& v) { this->x = v.x; this->y = v.y; this->z = v.z; }
    inline void operator+=(const rtos_vector3& v) { this->x += v.x; this->y += v.y; this->z += v.z; }
    inline void operator-=(const rtos_vector3& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; }
    inline void operator*=(const rtos_vector3& v) { this->x *= v.x; this->y *= v.y; this->z *= v.z; }
    inline void operator/=(const rtos_vector3& v) { this->x /= v.x; this->y /= v.y; this->z /= v.z; }
    inline void operator*=(const float f) { this->x *= f; this->y *= f; this->z *= f; }
    inline void operator/=(const float f) { this->x /= f; this->y /= f; this->z /= f; }
    inline rtos_vector3 operator -() { return rtos_vector3{ -this->x, -this->y, -this->z }; }
};

inline rtos_vector3 operator+(const rtos_vector3& v, const rtos_vector3& w) { return rtos_vector3{ v.x+w.x, v.y+w.y, v.z+w.z }; }
inline rtos_vector3 operator-(const rtos_vector3& v, const rtos_vector3& w) { return rtos_vector3{ v.x-w.x, v.y-w.y, v.z-w.z }; }
inline rtos_vector3 operator*(const rtos_vector3& v, const rtos_vector3& w) { return rtos_vector3{ v.x*w.x, v.y*w.y, v.z*w.z }; }
inline rtos_vector3 operator/(const rtos_vector3& v, const rtos_vector3& w) { return rtos_vector3{ v.x/w.x, v.y/w.y, v.z/w.z }; }
inline rtos_vector3 operator*(const rtos_vector3& v, const float f) { return rtos_vector3{ v.x/f, v.y/f, v.z/f }; }
inline rtos_vector3 operator/(const rtos_vector3& v, const float f) { return rtos_vector3{ v.x/f, v.y/f, v.z/f }; }
inline bool operator==(const rtos_vector3& v, const rtos_vector3& w) { return (v.x==w.x)&&(v.y==w.y)&&(v.z==w.z); }

// dot product
inline float operator^(const rtos_vector3& v, const rtos_vector3& w) { return (v.x*w.x)+(v.y*w.y)+(v.z*w.z); }
// cross product
inline rtos_vector3 operator%(const rtos_vector3& v, const rtos_vector3& w) { return rtos_vector3{ (v.y*w.z)-(v.z*w.y), (v.z*w.x)-(v.x*w.z), (v.x*w.y)-(v.y*w.x) }; }

inline float mag_sq(rtos_vector3& v) { return v.x * v.y * v.z; }
inline float mag(rtos_vector3& v) { return sqrt(mag_sq(v)); }
inline void norm_self(rtos_vector3& v) { v /= mag(v); }
rtos_vector3 norm_copy(rtos_vector3& v) { return v / mag(v); }

typedef rtos_vector3 rtos_colour;

struct rtos_vector2
{
    float u,v;

    inline void operator=(const rtos_vector2& a) { this->u = a.u; this->v = a.v; }
    inline void operator+=(const rtos_vector2& a) { this->u += a.u; this->v += a.v; }
    inline void operator-=(const rtos_vector2& a) { this->u -= a.u; this->v -= a.v; }
    inline void operator*=(const rtos_vector2& a) { this->u *= a.u; this->v *= a.v; }
    inline void operator/=(const rtos_vector2& a) { this->u /= a.u; this->v /= a.v; }
    inline void operator*=(const float f) { this->u *= f; this->v *= f; }
    inline void operator/=(const float f) { this->u /= f; this->v /= f; }
    inline rtos_vector2 operator -() { return rtos_vector2{ -this->u, -this->v }; }
};

inline rtos_vector2 operator+(const rtos_vector2& a, const rtos_vector2& b) { return rtos_vector2{ a.u+b.u, a.v+b.v }; }
inline rtos_vector2 operator-(const rtos_vector2& a, const rtos_vector2& b) { return rtos_vector2{ a.u-b.u, a.v-b.v }; }
inline rtos_vector2 operator*(const rtos_vector2& a, const rtos_vector2& b) { return rtos_vector2{ a.u*b.u, a.v*b.v }; }
inline rtos_vector2 operator/(const rtos_vector2& a, const rtos_vector2& b) { return rtos_vector2{ a.u/b.u, a.v/b.v }; }
inline rtos_vector2 operator*(const rtos_vector2& a, const float f) { return rtos_vector2{ a.u/f, a.v/f}; }
inline rtos_vector2 operator/(const rtos_vector2& a, const float f) { return rtos_vector2{ a.u/f, a.v/f}; }
inline bool operator==(const rtos_vector2& a, const rtos_vector2& b) { return (a.u==b.u)&&(a.v==b.v); }

// linear interpolate
inline rtos_vector3 lerp(const rtos_vector3& v, const rtos_vector3& w, const float f) { return rtos_vector3{ v.x+((w.x-v.x)*f), v.y+((w.y-v.y)*f), v.z+((w.z-v.z)*f) }; }
// reflect v in w
inline rtos_vector3 reflect(const rtos_vector3& v, const rtos_vector3& w) { rtos_vector3 u = v-(w*((v.x*w.x)+(v.y*w.y)+(v.z*w.z))*2); return u/mag(u); }
// perturb v parallel to w by d.u, and perpendicular to w and v by d.v
inline rtos_vector3 perturb(const rtos_vector3& v, const rtos_vector3& w, const rtos_vector2& d) { return v+(w*d.u)+((v%w)*d.v); }

struct rtos_ray
{
    rtos_vector3 origin, direction;
};

struct rtos_tri
{
    rtos_vector3 v1, v2, v3;
    rtos_vector3 v12, v13;
    rtos_vector3 n;
};

struct rtos_bounds
{
    rtos_vector3 min, max;
};

class rtos_gbuf
{
    // TODO
};