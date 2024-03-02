#include <gui/panel_memorymonitor.h>
#include <math.h>
#include <memory.h>
#include <colour.h>

using namespace nov;

void gui::nov_panel_memorymonitor::_draw draw_function_stub
{
    const uint32_t total_size = memory::memory_information.size;
    const uint32_t total_pixels = frame.size.u * frame.size.v;
    const float pixels_per_byte = (float)total_pixels / (float)total_size;

    nov_colour current_block_colour = nov_colour{ 255,64,64 };
    uint32_t pixel_index = 0;

    memory::nov_memory_frame* current = (memory::nov_memory_frame*)memory::memory_information.head;
    float carried_block_pixels = 0;
    while (current->next != 0x0)
    {
        uint32_t block_size = (uint32_t)(current->next) - (uint32_t)(current);
        float block_size_pixels = (float)(block_size) * pixels_per_byte;
        uint32_t block_size_rounded = (uint32_t)block_size_pixels;
        carried_block_pixels += block_size_pixels - block_size_rounded;
        if (carried_block_pixels > 1)
        {
            carried_block_pixels -= 1;
            block_size_rounded++;
        }
        for (uint32_t i = 0; i < block_size_rounded; i++)
        {
            // set pixel at index based on is_free
            uint32_t real_pixel_index = graphics::get_offset(nov_uvector2{ pixel_index % frame.size.u, pixel_index / frame.size.u } + frame.origin, framebuffer.size);
            graphics::set_pixel(real_pixel_index, current->is_free ? nov_colour_nearblack : current_block_colour, framebuffer);
            pixel_index++;
        }
        // update current block colour
        current_block_colour = nov_colour{ (uint8_t)(current_block_colour.x + 64), (uint8_t)(current_block_colour.y + 64), (uint8_t)(current_block_colour.z + 32) };
        current = current->next;
    }
}