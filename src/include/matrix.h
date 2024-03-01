#pragma once

#include <matrix/matrix2_2.h>
#include <matrix/matrix3_3.h>
#include <matrix/matrix4_4.h>

namespace nov
{
namespace matrix
{

typedef nov_matrix2_2<float> nov_fmatrix2;
typedef nov_matrix3_3<float> nov_fmatrix3;
typedef nov_matrix4_4<float> nov_fmatrix4;

template <typename T>
inline stream::nov_stream& operator<<(stream::nov_stream& stream, nov_matrix2_2<T> m)
{ 
    return stream << stream::mode::DEC << "matrix2_2" << stream::endl
                                           << "[ " << m.x_0 << ", " << m.y_0 << " ]" << stream::endl 
                                           << "[ " << m.x_1 << ", " << m.y_1 << " ]";
}

template <typename T>
inline stream::nov_stream& operator<<(stream::nov_stream& stream, nov_matrix3_3<T> m)
{ 
    return stream << stream::mode::DEC << "matrix3_3" << stream::endl
                                           << "[ " << m.x_0 << ", " << m.y_0 << ", " << m.z_0 << " ]" << stream::endl 
                                           << "[ " << m.x_1 << ", " << m.y_1 << ", " << m.z_1 << " ]" << stream::endl 
                                           << "[ " << m.x_2 << ", " << m.y_2 << ", " << m.z_2 << " ]";
}

template <typename T>
inline stream::nov_stream& operator<<(stream::nov_stream& stream, nov_matrix4_4<T> m)
{ 
    return stream << stream::mode::DEC << "matrix4_4" << stream::endl
                                           << "[ " << m.x_0 << ", " << m.y_0 << ", " << m.z_0 << ", " << m.w_0 << " ]" << stream::endl 
                                           << "[ " << m.x_1 << ", " << m.y_1 << ", " << m.z_1 << ", " << m.w_1 << " ]" << stream::endl 
                                           << "[ " << m.x_2 << ", " << m.y_2 << ", " << m.z_2 << ", " << m.w_2 << " ]" << stream::endl 
                                           << "[ " << m.x_3 << ", " << m.y_3 << ", " << m.z_3 << ", " << m.w_3 << " ]";
}

}
}
