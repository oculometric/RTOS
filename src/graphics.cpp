#include <graphics.h>
#include <serial.h>

using namespace nov::graphics;

// void nov_graphics_manager::draw_line(const nov_uvector2& a, const nov_uvector2& b, const nov_colour& col)
// {
//     nov_uvector2 minimum = a;
//     nov_uvector2 maximum = b;

//     if (a.u > b.u)
//     {
//         minimum = b;
//         maximum = a;
//     }

//     nov_uvector2 current = minimum;
//     nov_uvector2 offset = nov_uvector2{0,0};

//     float m = ((float)maximum.v-(float)minimum.v)/((float)maximum.u-(float)minimum.u);

//     if (m >= 1.0f)
//     {
//         while (current.u < maximum.u)
//         {
//             draw_pixel(current, col);
//             current.v++;
//             offset.v++;
//             current.u = (uint32_t)(((float)minimum.u)+((float)offset.v/m));
//             if (current.u > screen_size.u || current.v > screen_size.v) break;
//         }
//     } else if (m >= 0)
//     {
//         while (current.u < maximum.u)
//         {
//             draw_pixel(current, col);
//             current.u++;
//             offset.u++;
//             current.v = (uint32_t)(((float)minimum.v)+((float)offset.u*m));
//             if (current.u > screen_size.u || current.v > screen_size.v) break;
//         }
//     } else if (m >= -1.0f)
//     {
//         while (current.u < maximum.u)
//         {
//             draw_pixel(current, col);
//             current.u++;
//             offset.u++;
//             current.v = (uint32_t)(((float)minimum.v)+((float)offset.u*m));
//             if (current.u > screen_size.u || current.v > screen_size.v) break;
//         }
//     }
//     else if (m < -1.0f)
//     {
//         while (current.u < maximum.u)
//         {
//             draw_pixel(current, col);
//             current.v--;
//             offset.v++;
//             current.u = (uint32_t)(((float)minimum.u)-((float)offset.v/m));
//             if (current.u > screen_size.u || current.v > screen_size.v) break;
//         }
//     }
//     return;
// }

void nov::graphics::draw_box(const nov_uvector2& origin, const nov_uvector2& size, const nov_colour& col, const nov_framebuffer& framebuffer)
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

void nov::graphics::fill_box(const nov_uvector2& origin, const nov_uvector2& size, const nov_colour& col, const nov_framebuffer& framebuffer)
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
