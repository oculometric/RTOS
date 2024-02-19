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

inline void serial_println_uvector2(const nov_uvector2& v, uint32_t port) { serial_print((char*)"point(x:", port); serial_print_dec(v.u, port); serial_print((char*)",y:", port); serial_print_dec(v.v, port); serial_println((char*)")", port); }

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

}
}