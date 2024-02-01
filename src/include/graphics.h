#pragma once

#include <vbe.h>
#include <vector.h>
#include <serial.h>

inline void serial_println_uvector2(const nov_uvector2& v, uint32_t port) { serial_print((char*)"point(x:", port); serial_print_dec(v.u, port); serial_print((char*)",y:", port); serial_print_dec(v.v, port); serial_println((char*)")", port); }

class nov_graphics_manager
{
private:
    uint8_t* real_framebuffer;
    nov_uvector2 screen_size;
    uint32_t buffer_length;
    uint8_t bytes_per_pixel;
    inline uint32_t get_offset(const nov_uvector2& co);

public:
    nov_graphics_manager(vbe_mode_info* vbe_mode_info_block);

    void draw_pixel(const nov_uvector2& at, const nov_colour& col);
    inline void draw_pixel(uint32_t at, const nov_colour& col);
    void draw_line(const nov_uvector2& a, const nov_uvector2& b, const nov_colour& col);
    void draw_box(const nov_uvector2& tl, const nov_uvector2& br, const nov_colour& col);
};