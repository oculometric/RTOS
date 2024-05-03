#pragma once

#include <vector/vector3.h>

namespace nov
{
namespace graphics
{

struct Bounds
{
    vector::Vector3<float> min, max;
    vector::Vector3<float> center, radius;
};

void boundsMinMaxFromCenterRadius(Bounds& bounds);
void boundsCenterRadiusFromMinMax(Bounds& bounds);
void boundsFixMinmax(Bounds& bounds);

}
}