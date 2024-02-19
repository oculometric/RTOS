#pragma once

#include <stdint.h>

namespace nov
{
namespace vbe
{

#pragma pack (1)
struct nov_vbe_pm_info_block
{
    uint8_t p;
    uint8_t m;
    uint8_t i;
    uint8_t d;
    uint16_t entry_point;
    uint16_t pm_initialise;
    uint16_t bios_data_selector;
    uint16_t a0000_selector;
    uint16_t b0000_selector;
    uint16_t b8000_selector;
    uint16_t code_segment_selector;
    uint8_t in_protected_mode;
    uint8_t checksum;
};
#pragma pack (0)

#pragma pack (1)
struct nov_vbe_mode_info
{
    uint16_t mode_attributes;
    uint8_t window_a_attributes;
    uint8_t window_b_attributes;
    uint16_t window_granularity;
    uint16_t window_size;
    uint16_t window_a_start_segment;
    uint16_t window_b_start_segment;
    uint32_t window_function_ptr;
    uint16_t bytes_per_scanline;

    uint16_t x_resolution;
    uint16_t y_resolution;
    uint8_t char_size_x;
    uint8_t char_size_y;
    uint8_t num_memory_planes;
    uint8_t bits_per_pixel;
    uint8_t num_banks;
    uint8_t memory_model;
    uint8_t bank_size_kb;
    uint8_t num_image_planes;
    uint8_t reserved_0;

    uint8_t red_mask_size;
    uint8_t red_field_pos;
    uint8_t green_mask_size;
    uint8_t green_field_pos;
    uint8_t blue_mask_size;
    uint8_t blue_field_pos;
    uint8_t reserved_mask_size;
    uint8_t reserved_field_pos;
    uint8_t direct_colour_attributes;

    uint32_t flat_framebuffer_address;
    uint32_t reserved_1;
    uint16_t reserved_2;

    uint16_t bytes_per_scanline_linear;
    uint8_t num_images_banked;
    uint8_t num_images_linear;
    uint8_t red_mask_size_linear;
    uint8_t red_field_pos_linear;
    uint8_t green_mask_size_linear;
    uint8_t green_field_pos_linear;
    uint8_t blue_mask_size_linear;
    uint8_t blue_field_pos_linear;
    uint8_t reserved_mask_size_linear;
    uint8_t reserved_field_pos_linear;
    uint32_t max_pixel_clock;

    uint8_t reserved_3;
};
#pragma pack (0)

static inline void* find_pmid_block(void* start, uint32_t length)
{
    uint8_t* ptr = (uint8_t*)start;
    nov_vbe_pm_info_block* block;

    uint32_t offset = 0;
    while (offset < length)
    {
        block = (nov_vbe_pm_info_block*)(ptr+offset);
        if (block->p == 'P' && block->m == 'M' && block->i == 'I' && block->d == 'D')
        {
            return (void*)(ptr+offset);
        }
        offset++;
    }
    return (void*)0x0;
}

}
}