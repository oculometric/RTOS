#include <graphics.h>
#include <serial.h>

using namespace nov::graphics;

inline uint32_t nov_graphics_manager::get_offset(const nov_uvector2& co)
{
    return (this->screen_size.u*co.v)+co.u;
}

nov_graphics_manager::nov_graphics_manager(nov::vbe::nov_vbe_mode_info* vbe_mode_info_block)
{
    screen_size = nov_uvector2{ vbe_mode_info_block->x_resolution, vbe_mode_info_block->y_resolution };
    real_framebuffer = (uint8_t*)(vbe_mode_info_block->flat_framebuffer_address);
    bytes_per_pixel = vbe_mode_info_block->bits_per_pixel/8;
    buffer_length = screen_size.u * screen_size.v;
    serial_print((char*)"graphics manager inited with framebuffer located at ", COM1); serial_println_hex((uint32_t)real_framebuffer, COM1);
    serial_println_dec(buffer_length, COM1);
}

void nov_graphics_manager::draw_pixel(const nov_uvector2& at, const nov_colour& col)
{
    if (at.u > screen_size.u || at.v > screen_size.v) return;
    draw_pixel(get_offset(at), col);
}

void nov_graphics_manager::draw_pixel(uint32_t at, const nov_colour& col)
{
    if (at > buffer_length) return;
    real_framebuffer[(at*bytes_per_pixel)] = col.z;
    real_framebuffer[(at*bytes_per_pixel)+1] = col.y;
    real_framebuffer[(at*bytes_per_pixel)+2] = col.x;
}

void nov_graphics_manager::draw_line(const nov_uvector2& a, const nov_uvector2& b, const nov_colour& col)
{
    nov_uvector2 minimum = a;
    nov_uvector2 maximum = b;

    if (a.u > b.u)
    {
        minimum = b;
        maximum = a;
    }

    nov_uvector2 current = minimum;
    nov_uvector2 offset = nov_uvector2{0,0};

    float m = ((float)maximum.v-(float)minimum.v)/((float)maximum.u-(float)minimum.u);

    if (m >= 1.0f)
    {
        while (current.u < maximum.u)
        {
            draw_pixel(current, col);
            current.v++;
            offset.v++;
            current.u = (uint32_t)(((float)minimum.u)+((float)offset.v/m));
            if (current.u > screen_size.u || current.v > screen_size.v) break;
        }
    } else if (m >= 0)
    {
        while (current.u < maximum.u)
        {
            draw_pixel(current, col);
            current.u++;
            offset.u++;
            current.v = (uint32_t)(((float)minimum.v)+((float)offset.u*m));
            if (current.u > screen_size.u || current.v > screen_size.v) break;
        }
    } else if (m >= -1.0f)
    {
        while (current.u < maximum.u)
        {
            draw_pixel(current, col);
            current.u++;
            offset.u++;
            current.v = (uint32_t)(((float)minimum.v)+((float)offset.u*m));
            if (current.u > screen_size.u || current.v > screen_size.v) break;
        }
    }
    else if (m < -1.0f)
    {
        while (current.u < maximum.u)
        {
            draw_pixel(current, col);
            current.v--;
            offset.v++;
            current.u = (uint32_t)(((float)minimum.u)-((float)offset.v/m));
            if (current.u > screen_size.u || current.v > screen_size.v) break;
        }
    }
    return;
}

void nov_graphics_manager::draw_box(const nov_uvector2& tl, const nov_uvector2& br, const nov_colour& col)
{
    uint32_t offset = get_offset(tl);
    uint32_t br_offset = get_offset(br);
    nov_uvector2 tracker = tl;

    while (offset < br_offset)
    {
        draw_pixel(offset, col);
        offset++;
        tracker.u++;
        if (tracker.u > br.u)
        {
            tracker.u = tl.u;
            tracker.v++;
            offset = get_offset(tracker);
        }
    }

}
