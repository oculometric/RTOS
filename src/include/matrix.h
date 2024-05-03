#pragma once

#include <matrix/matrix2_2.h>
#include <matrix/matrix3_3.h>
#include <matrix/matrix4_4.h>

namespace nov
{
namespace matrix
{

typedef Matrix2_2<float> FMatrix2;
typedef Matrix3_3<float> FMatrix3;
typedef Matrix4_4<float> FMatrix4;

template <typename T>
inline stream::Stream& operator<<(stream::Stream& stream, Matrix2_2<T> m)
{ 
    return stream << stream::Mode::DEC << "matrix2_2" << stream::endl
                                           << "[ " << m.x_0 << ", " << m.y_0 << " ]" << stream::endl 
                                           << "[ " << m.x_1 << ", " << m.y_1 << " ]";
}

template <typename T>
inline stream::Stream& operator<<(stream::Stream& stream, Matrix3_3<T> m)
{ 
    return stream << stream::Mode::DEC << "matrix3_3" << stream::endl
                                           << "[ " << m.x_0 << ", " << m.y_0 << ", " << m.z_0 << " ]" << stream::endl 
                                           << "[ " << m.x_1 << ", " << m.y_1 << ", " << m.z_1 << " ]" << stream::endl 
                                           << "[ " << m.x_2 << ", " << m.y_2 << ", " << m.z_2 << " ]";
}

template <typename T>
inline stream::Stream& operator<<(stream::Stream& stream, Matrix4_4<T> m)
{ 
    return stream << stream::Mode::DEC << "matrix4_4" << stream::endl
                                           << "[ " << m.x_0 << ", " << m.y_0 << ", " << m.z_0 << ", " << m.w_0 << " ]" << stream::endl 
                                           << "[ " << m.x_1 << ", " << m.y_1 << ", " << m.z_1 << ", " << m.w_1 << " ]" << stream::endl 
                                           << "[ " << m.x_2 << ", " << m.y_2 << ", " << m.z_2 << ", " << m.w_2 << " ]" << stream::endl 
                                           << "[ " << m.x_3 << ", " << m.y_3 << ", " << m.z_3 << ", " << m.w_3 << " ]";
}

}
}
