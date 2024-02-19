#include <colour.h>

namespace nov
{
namespace colour
{

nov_fvector3 rgb_to_hsv(const nov_fvector3& rgb)
{
    nov_fvector3 hsv { 0,0,0 };
    float mx = max(rgb.x, rgb.y, rgb.z);
    float mn = min(rgb.x, rgb.y, rgb.z);

    hsv.z = mx;
    if (mx != 0) hsv.y = (mx-mn)/mx;
    if (hsv.z == 0) return hsv;

    float d = mx-mn;
    if (rgb.x == mx) hsv.x = (rgb.y-rgb.z)/d;
    else if (rgb.y == mx) hsv.x = 2+((rgb.z-rgb.x)/d);
    else if (rgb.z == mx) hsv.x = 4+((rgb.x-rgb.y)/d);
    hsv.x /= 6.0f;
    
    if (hsv.x < 0) hsv.x += 1.0f;
    if (hsv.x > 1) hsv.x -= 1.0f;
    
    return hsv;
}

nov_fvector3 hsv_to_rgb(const nov_fvector3& hsv)
{
    if (hsv.y == 0) return nov_fvector3{ hsv.z,hsv.z,hsv.z };
    uint8_t sextant = (uint8_t)(hsv.x*6.0f);
    float fraction = (hsv.x*6.0f)-sextant;
    float p = hsv.z*(1.0f-hsv.y);
    float q = hsv.z*(1.0f-(hsv.y*fraction));
    float t = hsv.z*(1.0f-(hsv.y*(1.0f-fraction)));
    switch (sextant)
    {
    case 0: return { hsv.z,t,p };
    case 1: return { q,hsv.z,p };
    case 2: return { p,hsv.z,t };
    case 3: return { p,q,hsv.z };
    case 4: return { t,p,hsv.z };
    case 5: return { hsv.z,p,q };
    }
    return nov_fvector3 { 0,0,0 };
}

}
}
