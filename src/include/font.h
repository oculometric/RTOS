#pragma once

#include <stdint.h>

namespace nov
{

struct nov_font
{
    uint8_t char_width = 5;
    uint8_t char_height = 8;
    
    uint8_t* bitmap = 0;
    uint16_t bitmap_width;
    uint16_t bitmap_height;
    uint16_t tiles_per_row;
    uint16_t tiles_per_column;
};

}