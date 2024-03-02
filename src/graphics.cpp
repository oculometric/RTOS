#include <graphics.h>

namespace nov
{
namespace graphics
{

using namespace vector;

void draw_box(const nov_uvector2& origin, const nov_uvector2& size, const nov_colour& col, const nov_framebuffer& framebuffer)
{
    uint32_t top_left = get_offset(origin, framebuffer.size);
    uint32_t top_right = (top_left + size.u) - 1;
    uint32_t bottom_left = top_left;
    uint32_t bottom_right = top_right;

    for (uint32_t t = 0; t < size.v; t++)
    {
        set_pixel(bottom_left, col, framebuffer);
        set_pixel(bottom_right, col, framebuffer);

        bottom_left += framebuffer.size.u;
        bottom_right += framebuffer.size.u;
    }

    top_right = top_left;
    bottom_left -= framebuffer.size.u;

    for (uint32_t t = 0; t < size.u; t++)
    {
        set_pixel(top_right, col, framebuffer);
        set_pixel(bottom_left, col, framebuffer);

        top_right++;
        bottom_left++;
    }
}

void fill_box(const nov_uvector2& origin, const nov_uvector2& size, const nov_colour& col, const nov_framebuffer& framebuffer)
{
    uint32_t offset = get_offset(origin, framebuffer.size);
    uint32_t end_offset = get_offset(origin+size-nov_uvector2{ 1,1 }, framebuffer.size);
    uint32_t x = 0;

    while (offset <= end_offset)
    {
        set_pixel(offset, col, framebuffer);
        offset++;
        x++;
        if (x >= size.u)
        {
            x = 0;
            offset -= size.u;
            offset += framebuffer.size.u;
        }
    }
}

void draw_line(const nov_uvector2& start, const nov_uvector2& end, const nov_colour& col, const nov_framebuffer& framebuffer)
{
    nov_uvector2 minimum = start;
    nov_uvector2 maximum = end;

    if (start.u > end.u)
    {
        minimum = end;
        maximum = start;
    }

    nov_uvector2 current = minimum;
    nov_uvector2 offset = nov_uvector2{0,0};

    float m = ((float)maximum.v-(float)minimum.v)/((float)maximum.u-(float)minimum.u);

    if (maximum.u == minimum.u)
    {
        if (start.v > end.v)
        {
            minimum = end;
            maximum = start;
        }
        else
        {
            minimum = start;
            maximum = end;
        }
        current = minimum;
        while (current.v < maximum.v)
        {
            set_pixel(get_offset(current, framebuffer.size), col, framebuffer);
            current.v++;
        }
    } else if (m >= 1.0f)
    {
        while (current.u < maximum.u)
        {
            set_pixel(get_offset(current, framebuffer.size), col, framebuffer);
            current.v++;
            offset.v++;
            current.u = (uint32_t)(((float)minimum.u)+((float)offset.v/m));
            if (current.u > framebuffer.size.u || current.v > framebuffer.size.v) break;
        }
    } else if (m >= 0)
    {
        while (current.u < maximum.u)
        {
            set_pixel(get_offset(current, framebuffer.size), col, framebuffer);
            current.u++;
            offset.u++;
            current.v = (uint32_t)(((float)minimum.v)+((float)offset.u*m));
            if (current.u > framebuffer.size.u || current.v > framebuffer.size.v) break;
        }
    } else if (m >= -1.0f)
    {
        while (current.u < maximum.u)
        {
            set_pixel(get_offset(current, framebuffer.size), col, framebuffer);
            current.u++;
            offset.u++;
            current.v = (uint32_t)(((float)minimum.v)+((float)offset.u*m));
            if (current.u > framebuffer.size.u || current.v > framebuffer.size.v) break;
        }
    }
    else if (m < -1.0f)
    {
        while (current.u < maximum.u)
        {
            set_pixel(get_offset(current, framebuffer.size), col, framebuffer);
            current.v--;
            offset.v++;
            current.u = (uint32_t)(((float)minimum.u)-((float)offset.v/m));
            if (current.u > framebuffer.size.u || current.v > framebuffer.size.v) break;
        }
    }
    return;
}

}
}