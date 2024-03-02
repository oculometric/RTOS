#pragma once

#include <vbe.h>
#include <vector.h>
#include <serial.h>

namespace nov
{
namespace graphics
{

using namespace nov::vector;

struct nov_framebuffer
{
    uint8_t* address;
    nov_uvector2 size;
    uint8_t bytes_per_pixel;
};

static inline void set_pixel(uint32_t at, const nov_colour& col, const nov_framebuffer& framebuffer) 
{
    framebuffer.address[(at*framebuffer.bytes_per_pixel)] = col.z;
    framebuffer.address[(at*framebuffer.bytes_per_pixel)+1] = col.y;
    framebuffer.address[(at*framebuffer.bytes_per_pixel)+2] = col.x;
}

static inline uint32_t get_offset(const nov_uvector2& co, const nov_uvector2& framebuffer_size)
{
    return (co.u % framebuffer_size.u) + ((co.v % framebuffer_size.v) * framebuffer_size.u);
}

void draw_box(const nov_uvector2& origin, const nov_uvector2& size, const nov_colour& col, const nov_framebuffer& framebuffer);
void fill_box(const nov_uvector2& origin, const nov_uvector2& size, const nov_colour& col, const nov_framebuffer& framebuffer);
void draw_line(const nov_uvector2& start, const nov_uvector2& end, const nov_colour& col, const nov_framebuffer& framebuffer);

}
}