// represents a mathematical vector with 3 components
struct nov_vector3
{
    float x, y, z;

    inline void operator=(const nov_vector3 & a) { this->x = a.x; this->y = a.y; this->z = a.z; }
    inline void operator+=(const nov_vector3 & a) { this->x += a.x; this->y += a.y; this->z += a.z; }
    inline void operator-=(const nov_vector3 & a) { this->x -= a.x; this->y -= a.y; this->z -= a.z; }
    inline void operator*=(const nov_vector3 & a) { this->x *= a.x; this->y *= a.y; this->z *= a.z; }
    inline void operator/=(const nov_vector3 & a) { this->x /= a.x; this->y /= a.y; this->z /= a.z; }
    inline void operator*=(const float f) { this->x *= f; this->y *= f; this->z *= f; }
    inline void operator/=(const float f) { this->x /= f; this->y /= f; this->z /= f; }
    inline nov_vector3 operator -() { return nov_vector3{ -this->x, -this->y, -this->z }; }
};

inline nov_vector3 operator+(const nov_vector3 & a, const nov_vector3 & b) { return nov_vector3{ a.x+b.x, a.y+b.y, a.z+b.z }; }
inline nov_vector3 operator-(const nov_vector3 & a, const nov_vector3 & b) { return nov_vector3{ a.x-b.x, a.y-b.y, a.z-b.z }; }
inline nov_vector3 operator*(const nov_vector3 & a, const nov_vector3 & b) { return nov_vector3{ a.x*b.x, a.y*b.y, a.z*b.z }; }
inline nov_vector3 operator/(const nov_vector3 & a, const nov_vector3 & b) { return nov_vector3{ a.x/b.x, a.y/b.y, a.z/b.z }; }
inline nov_vector3 operator*(const nov_vector3 & a, const float f) { return nov_vector3{ a.x*f, a.y*f, a.z*f }; }
inline nov_vector3 operator/(const nov_vector3 & a, const float f) { return nov_vector3{ a.x/f, a.y/f, a.z/f }; }
inline bool operator==(const nov_vector3 & a, const nov_vector3 & b) { return (a.x==b.x) && (a.y==b.y) && (a.z==b.z); }

// dot product between two vectors. this represents the magnitude of the two vectors multiplied together,
// multiplied by the cosine of the angle between them
inline float operator^(const nov_vector3 & a, const nov_vector3 & b) { return (a.x*b.x)+(a.y*b.y)+(a.z*b.z); }
// cross product. this represents the magnitude of the two vectors multiplied together, 
// multiplied by the sine of the angle between them, multiplied by the vector which is normal to
// the two input vectors. when oriented with the two vectors pointing as upward as possible visually,
// left vector cross right vector will point directly into the camera
inline nov_vector3 operator%(const nov_vector3 & a, const nov_vector3 & b) { return nov_vector3{ (a.y*b.z)-(a.z*b.y), (a.z*b.x)-(a.x*b.z), (a.x*b.y)-(a.y*b.x) }; }

// calculate square magnitude of a vector
inline float mag_sq(const nov_vector3 & a) { return (a.x*a.x)+(a.y*a.y)+(a.z*a.z); }
// calcualte magnitude of a vector
inline float mag(const nov_vector3 & a) { return sqrt((a.x*a.x)+(a.y*a.y)+(a.z*a.z)); }

// normalise a vector in place
inline void norm_self(nov_vector3 & a) { a /= mag(a); }
// calculate a normalised copy of a vector
inline nov_vector3 norm_copy(const nov_vector3 & a) { return a / mag(a); }

typedef nov_vector3 nov_colour;

// represents a 2 dimensional vector, with associated operations
struct nov_vector2
{
    float u,v;

    inline void operator=(const nov_vector2 & a) { this->u = a.u; this->v = a.v; }
    inline void operator+=(const nov_vector2 & a) { this->u += a.u; this->v += a.v; }
    inline void operator-=(const nov_vector2 & a) { this->u -= a.u; this->v -= a.v; }
    inline void operator*=(const nov_vector2 & a) { this->u *= a.u; this->v *= a.v; }
    inline void operator/=(const nov_vector2 & a) { this->u /= a.u; this->v /= a.v; }
    inline void operator*=(const float f) { this->u *= f; this->v *= f; }
    inline void operator/=(const float f) { this->u /= f; this->v /= f; }
    inline nov_vector2 operator -() { return nov_vector2{ -this->u, -this->v }; }
};

inline nov_vector2 operator+(const nov_vector2 & a, const nov_vector2 & b) { return nov_vector2{ a.u+b.u, a.v+b.v }; }
inline nov_vector2 operator-(const nov_vector2 & a, const nov_vector2 & b) { return nov_vector2{ a.u-b.u, a.v-b.v }; }
inline nov_vector2 operator*(const nov_vector2 & a, const nov_vector2 & b) { return nov_vector2{ a.u*b.u, a.v*b.v }; }
inline nov_vector2 operator/(const nov_vector2 & a, const nov_vector2 & b) { return nov_vector2{ a.u/b.u, a.v/b.v }; }
inline nov_vector2 operator*(const nov_vector2 & a, const float f) { return nov_vector2{ a.u*f, a.v*f }; }
inline nov_vector2 operator/(const nov_vector2 & a, const float f) { return nov_vector2{ a.u/f, a.v/f }; }
inline bool operator==(const nov_vector2 & a, const nov_vector2 & b) { return (a.u==b.u) && (a.v==b.v); }

// dot product between two vectors. this represents the magnitude of the two vectors multiplied together,
// multiplied by the cosine of the angle between them
inline float operator^(const nov_vector2 & a, const nov_vector2 & b) { return (a.u*b.u)+(a.v*b.v); }

// calculate square magnitude of a vector
inline float mag_sq(const nov_vector2 & a) { return (a.u*a.u)+(a.v*a.v); }
// calcualte magnitude of a vector
inline float mag(const nov_vector2 & a) { return sqrt((a.u*a.u)+(a.v*a.v)); }

// normalise a vector in place
inline void norm_self(nov_vector3 & a) { a /= mag(a); }
// calculate a normalised copy of a vector
inline nov_vector3 norm_copy(const nov_vector3 & a) { return a / mag(a); }

// linear interpolate between two vectors
inline nov_vector3 lerp(const nov_vector3 & a, const nov_vector3 & b, const float f) { nov_vector3{ a.x+((b.x-a.x)*f), a.y+((b.y-a.y)*f), a.z+((b.z-a.z)*f) }; }
// linear interpolate between two vectors
inline nov_vector2 lerp(const nov_vector2 & a, const nov_vector2 & b, const float f) { nov_vector2{ a.u+((b.u-a.u)*f), a.v+((b.v-a.v)*f) }; }
// reflect v in w, where w can be interpreted as a vector normal to a mirror in which v is reflected
inline nov_vector3 reflect(const nov_vector3 & a, const nov_vector3 & b) { return a-(b*(a^b)*2); }