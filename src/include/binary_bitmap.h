#pragma once

#include <stdint.h>

#define NOV_BINARY_BITMAP_HEADER_CHECKSUM ('n' | ('b' << 8) | ('b' << 16) | ('h' << 24))


namespace nov
{
namespace file
{

struct nov_binary_bitmap_header
{
    uint32_t checksum;

    uint32_t data_offset;
    uint8_t bits_per_pixel;

    uint16_t image_width;
    uint16_t image_height;
    uint32_t image_size;
};

}
}