#pragma once

#include <stdint.h>
#include <vector.h>

namespace nov
{
namespace graphics
{
    
struct Framebuffer
{
    uint8_t* address;
    vector::UVector2 size;
    uint8_t bytes_per_pixel;
};

}
}