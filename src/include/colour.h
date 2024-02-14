#pragma once

#include <vector.h>

using namespace nov::vector;

namespace nov
{
namespace colour
{

#define nov_colour_red nov_colour{ 255,0,0 }
#define nov_colour_green nov_colour{ 0,255,0 }
#define nov_colour_blue nov_colour{ 0,0,255 }
#define nov_colour_yellow nov_colour{ 255,255,0 }
#define nov_colour_cyan nov_colour{ 0,255,255 }
#define nov_colour_magenta nov_colour{ 255,0,255 }

#define nov_colour_vapor nov_colour{ 255,191,38 }
#define nov_colour_gold nov_colour{ 232,144,2 }
#define nov_colour_nearblack nov_colour{ 3,1,0 }

const static nov_fvector3 luminance_vector{ 0.2126,0.7152,0.0722 };

nov_fvector3 rgb_to_hsv(const nov_fvector3& rgb);
nov_fvector3 hsv_to_rgb(const nov_fvector3& hsv);

inline float luminance(const nov_fvector3& col)
{
    return col^luminance_vector;
}

inline float luminance(const nov_colour& col)
{
    nov_fvector3 fcol = nov_fvector3{ col.x, col.y, col.z };
    return fcol^luminance_vector;
}

nov_fvector3 srgb_to_linear(const nov_fvector3& col);
nov_fvector3 linear_to_srgb(const nov_fvector3& col);

nov_fvector3 tonemap(const nov_fvector3& col);

}
}