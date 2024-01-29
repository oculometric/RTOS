#include <graphics.h>
#include <serial.h>

inline uint32_t nov_graphics_manager::get_offset(const nov_uvector2& co)
{
    return ((this->screen_size.u*co.v)+co.u)*bytes_per_pixel;
}

nov_graphics_manager::nov_graphics_manager(vbe_mode_info* vbe_mode_info_block)
{
    screen_size = nov_uvector2{ vbe_mode_info_block->x_resolution, vbe_mode_info_block->y_resolution };
    real_framebuffer = (uint8_t*)(vbe_mode_info_block->flat_framebuffer_address);
    bytes_per_pixel = vbe_mode_info_block->bits_per_pixel/8;
    serial_print((char*)"graphics manager inited with framebuffer located at ", COM1); serial_println_hex((uint32_t)real_framebuffer, COM1);
}

void nov_graphics_manager::draw_pixel(const nov_uvector2& at, const nov_colour& col)
{
    draw_pixel(get_offset(at), col);
}

void nov_graphics_manager::draw_pixel(uint32_t at, const nov_colour& col)
{
    real_framebuffer[at] = (uint8_t)(col.z*255);
    real_framebuffer[at+1] = (uint8_t)(col.y*255);
    real_framebuffer[at+2] = (uint8_t)(col.x*255);
}

void nov_graphics_manager::draw_line(const nov_uvector2& a, const nov_uvector2& b, const nov_colour& col)
{
    
}

void nov_graphics_manager::draw_box(const nov_uvector2& tl, const nov_uvector2& br, const nov_colour& col)
{
    uint32_t offset = get_offset(tl);
    uint32_t br_offset = get_offset(br);
    nov_uvector2 tracker = tl;

    while (offset < br_offset)
    {
        draw_pixel(offset, col);
        offset += bytes_per_pixel;
        tracker.u++;
        if (tracker.u > br.u)
        {
            tracker.u = tl.u;
            tracker.v++;
            offset = get_offset(tracker);
        }
    }

}
