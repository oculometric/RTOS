#pragma once

#include <stdint.h>

#include <vector/vector3.h>
#include <vector/vector2.h>
#include <vector/vector4.h>

#include <stream.h>

namespace nov
{
namespace vector
{

typedef Vector3<uint8_t> Colour;
typedef Vector3<float> FVector3;
typedef Vector3<uint32_t> UVector3;
typedef Vector3<int32_t> IVector3;

typedef Vector2<float> FVector2;
typedef Vector2<uint32_t> UVector2;
typedef Vector2<int32_t> IVector2;

typedef Vector4<uint8_t> TColour;
typedef Vector4<float> FVector4;
typedef Vector4<uint32_t> UVector4;
typedef Vector4<int32_t> IVector4;

template <typename T>
inline stream::Stream& operator<<(stream::Stream& stream, Vector3<T> v)
{ return stream << stream::Mode::DEC << "vector3{ " << v.x << ", " << v.y << ", " << v.z << " }"; }

template <typename T>
inline stream::Stream& operator<<(stream::Stream& stream, Vector2<T> v)
{ return stream << stream::Mode::DEC << "vector2{ " << v.u << ", " << v.v << " }"; }

template <typename T>
inline stream::Stream& operator<<(stream::Stream& stream, Vector4<T> v)
{ return stream << stream::Mode::DEC << "vector4{ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " }"; }

}
}