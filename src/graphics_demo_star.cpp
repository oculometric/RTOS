#include <graphics.h>

namespace nov
{
namespace graphics
{

void drawStar(Framebuffer buffer, vector::UVector2 uv, Colour foreground, Colour background, bool skip_background)
{
    // pointiness = 16
    const float c = 16;
    const float a = buffer.size.u;
    const float b = buffer.size.v;

    // cache constants
    const float scale_x_inv = (a * uv.u) / (2 * (c - 1));
    const float scale_y = (2 * (c - 1)) / (b * uv.v);
    const float half_x = a / 2.0f;
    const float half_y = b / 2.0f;

    const float y_limit_0 = half_y * (1 - uv.v);
    const float y_limit_1 = half_y * (1 + uv.v);

    // iterate over pixels
    float value = 0;
    uint32_t pixel_index = 0;
    bool inside_star = false;
    uint32_t first_intersection = b;
    uint32_t second_intersection = b;
    for (uint32_t y = 0; y < b; y++)
    {
        inside_star = false;

        // calculate the first intersection with the y = y line
        if (y < y_limit_0 || y > y_limit_1)
        {
            // there are no intersections
            first_intersection = b;
            second_intersection = b;
        }
        else
        {
            // calculate the y component value and solve for the two intersections
            value = (abs(y - half_y) * scale_y + 1.0f);
            value = ((c / value) - 1.0f) * scale_x_inv;
            first_intersection = (uint32_t)(half_x - value);
            second_intersection = (uint32_t)(half_x + value) - 1;
        }

        if (skip_background)
        {
            pixel_index = (y * buffer.size.u) + first_intersection;
            for (uint32_t x = first_intersection; x < second_intersection; x++)
            {
                pixel_index++;
                setPixel(pixel_index, foreground, buffer);
            }
        }
        else
        {
            for (uint32_t x = 0; x < buffer.size.u; x++)
            {
                if (x == first_intersection || x == second_intersection) inside_star = !inside_star;
                setPixel(pixel_index, inside_star ? foreground : background, buffer);
                pixel_index++;
            }
        }
    }
}
    
}
}