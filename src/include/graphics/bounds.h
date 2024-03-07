#pragma once

#include <vector/vector3.h>

namespace nov
{
namespace graphics
{

struct nov_bounds
{
    vector::nov_vector3<float> min, max;
    vector::nov_vector3<float> center, radius;
};

void bounds_mm_from_cr(nov_bounds& bounds);
void bounds_cr_from_mm(nov_bounds& bounds);
void bounds_fix_minmax(nov_bounds& bounds);

}
}