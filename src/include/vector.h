#pragma once

#include <stdint.h>

#include <vector/vector3.h>
#include <vector/vector2.h>
#include <vector/vector4.h>

namespace nov
{
namespace vector
{

typedef nov_vector3<uint8_t> nov_colour;
typedef nov_vector3<float> nov_fvector3;
typedef nov_vector3<uint32_t> nov_uvector3;
typedef nov_vector3<int32_t> nov_ivector3;

typedef nov_vector2<float> nov_fvector2;
typedef nov_vector2<uint32_t> nov_uvector2;
typedef nov_vector2<int32_t> nov_ivector2;

typedef nov_vector4<uint8_t> nov_tcolour;
typedef nov_vector4<float> nov_fvector4;
typedef nov_vector4<uint32_t> nov_uvector4;
typedef nov_vector4<int32_t> nov_ivector4;

}
}