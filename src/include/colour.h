#pragma once

#include <vector.h>

using namespace nov::vector;

namespace nov
{
namespace colour
{

#define nov_colour_red Colour{ 255,0,0 }
#define nov_colour_green Colour{ 0,255,0 }
#define nov_colour_blue Colour{ 0,0,255 }
#define nov_colour_yellow Colour{ 255,255,0 }
#define nov_colour_cyan Colour{ 0,255,255 }
#define nov_colour_magenta Colour{ 255,0,255 }
#define nov_colour_white Colour{ 255,255,255 }
#define nov_colour_black Colour{ 0,0,0 }

#define nov_colour_gold Colour{ 242,161,26 }
#define nov_colour_carmine Colour{ 207, 46, 29 }
#define nov_colour_indigo Colour{ 100, 46, 150 }
#define nov_colour_nearblack Colour{ 3,1,0 }

const static FVector3 luminance_vector{ 0.2126,0.7152,0.0722 };

FVector3 RGBToHSV(const FVector3& rgb);
FVector3 HSVToRGB(const FVector3& hsv);

inline float luminance(const FVector3& col)
{
    return col^luminance_vector;
}

inline float luminance(const Colour& col)
{
    FVector3 fcol = FVector3{ col.x/255.0f, col.y/255.0f, col.z/255.0f };
    return fcol^luminance_vector;
}

FVector3 srgbToLinear(const FVector3& col);
FVector3 linearToSrgb(const FVector3& col);

FVector3 tonemap(const FVector3& col);

}
}