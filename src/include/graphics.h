#pragma once

#include <vbe.h>
#include <vector.h>
#include <graphics/mesh.h>
#include <graphics/bounds.h>
#include <graphics/material.h>
#include <font.h>

namespace nov
{
namespace graphics
{

using namespace vector;

struct Framebuffer
{
    uint8_t* address;
    UVector2 size;
    uint8_t bytes_per_pixel;
};

static inline void setPixel(uint32_t at, const Colour& col, const Framebuffer& framebuffer) 
{
    framebuffer.address[(at*framebuffer.bytes_per_pixel)] = col.z;
    framebuffer.address[(at*framebuffer.bytes_per_pixel)+1] = col.y;
    framebuffer.address[(at*framebuffer.bytes_per_pixel)+2] = col.x;
}

static inline uint32_t getOffset(const UVector2& co, const UVector2& framebuffer_size)
{
    return (co.u % framebuffer_size.u) + ((co.v % framebuffer_size.v) * framebuffer_size.u);
}

void drawBox(const UVector2& origin, const UVector2& size, const Colour& col, const Framebuffer& framebuffer);
void fillBox(const UVector2& origin, const UVector2& size, const Colour& col, const Framebuffer& framebuffer);
void drawLine(const UVector2& start, const UVector2& end, const Colour& col, const Framebuffer& framebuffer);
void drawCharacter(char chr, const UVector2& origin, const Colour& col, const Font& font, const Framebuffer& framebuffer);

}
}