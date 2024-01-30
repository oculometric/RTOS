#pragma once

#include <vector.h>

#define nov_colour_red nov_colour{ 1,0,0 }
#define nov_colour_green nov_colour{ 0,1,0 }
#define nov_colour_blue nov_colour{ 0,0,1 }
#define nov_colour_yellow nov_colour{ 1,1,0 }
#define nov_colour_cyan nov_colour{ 0,1,1 }
#define nov_colour_magenta nov_colour{ 1,0,1 }

#define nov_colour_vapor nov_colour{ 1.0f,0.75f,0.15f }

nov_colour rgb_to_hsv(const nov_colour& rgb);
nov_colour hsv_to_rgb(const nov_colour& hsv);

inline float luminance(const nov_colour& col)
{
    return col^nov_colour{ 0.2126,0.7152,0.0722 };
}

nov_colour srgb_to_linear(const nov_colour& col);
nov_colour linear_to_srgb(const nov_colour& col);

nov_colour tonemap(const nov_colour& col);