#pragma once

#include "vbe.h"
#include "vector.h"

class nov_graphics_manager
{
private:
    uint8_t* real_framebuffer;
    nov_uvector2 screen_size;
    uint8_t bytes_per_pixel;
    inline uint32_t get_offset(const nov_uvector2& co);

public:
    nov_graphics_manager(vbe_mode_info* vbe_mode_info_block);

    void draw_pixel(const nov_uvector2& at, const nov_colour& col);
    inline void draw_pixel(uint32_t at, const nov_colour& col);
    void draw_line(const nov_uvector2& a, const nov_uvector2& b, const nov_colour& col);
    void draw_box(const nov_uvector2& tl, const nov_uvector2& br, const nov_colour& col);
};