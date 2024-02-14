#include <window.h>

#include <serial.h>

namespace nov
{
namespace gui
{

static inline uint16_t get_slice_offset(const vector::nov_uvector2& local_position, const vector::nov_uvector2& size)
{
    uint8_t bitpacked = // in order from lowest bit to highest bit
        ((local_position.u < WINDOW_SLICE_SIZE)) |
        ((local_position.v < WINDOW_SLICE_SIZE) << 1) |
        ((size.u-local_position.u < WINDOW_SLICE_SIZE) << 2) |
        ((size.v-local_position.v < WINDOW_SLICE_SIZE) << 3);
    
    switch (bitpacked)
    {
    case 0b0000: return WINDOW_SOLID*WINDOW_SLICE_AREA;
    case 0b0001: return WINDOW_LEFTEDGE*WINDOW_SLICE_AREA;
    case 0b0010: return WINDOW_TOPEDGE*WINDOW_SLICE_AREA;
    case 0b0011: return WINDOW_TOPLEFTCORNER*WINDOW_SLICE_AREA;
    case 0b0100: return WINDOW_RIGHTEDGE*WINDOW_SLICE_AREA;
    case 0b0110: return WINDOW_TOPRIGHTCORNER*WINDOW_SLICE_AREA;
    case 0b1000: return WINDOW_BOTTOMEDGE*WINDOW_SLICE_AREA;
    case 0b1100: return WINDOW_BOTTOMRIGHTCORNER*WINDOW_SLICE_AREA;
    case 0b1001: return WINDOW_BOTTOMLEFTCORNER*WINDOW_SLICE_AREA;
    default: return WINDOW_SOLID*WINDOW_SLICE_AREA;
    }
}

void draw_window(const nov_uvector2& origin, const nov_uvector2& size, uint8_t* buffer, const nov_uvector2& buffer_size)
{
    nov_uvector2 local_position;
    nov_uvector2 global_position = origin;

    uint32_t global_index = (origin.v*buffer_size.u)+origin.u;
    uint16_t mod_index = 0;
    uint16_t slice_offset = 0;
    uint8_t value = 0;
    nov_colour colour;
    while (local_position.u < size.u && local_position.v < size.v)
    {
        slice_offset = get_slice_offset(local_position, size);
        value = sliced_window[slice_offset+((local_position.v%5)*5)+(local_position.u%5)];
        colour = colour_palette[value];
        buffer[(global_index*3)+0] = colour.z;
        buffer[(global_index*3)+1] = colour.y;
        buffer[(global_index*3)+2] = colour.x;

        local_position.u++;
        global_position.u++;
        global_index++;
        if (local_position.u >= size.u)
        {
            local_position.u = 0;
            local_position.v++;

            global_position.u = origin.u;
            global_position.v++;

            global_index -= size.u;
            global_index += buffer_size.u;
        }
        mod_index++;
        mod_index = mod_index%5;
    }
}

#include <colour.h>

void nov_boxed_text::draw()
{
    if (panel_size.u < 3 || panel_size.v < 3) return;

    nov_vector3<uint8_t> col;
    col.x = nov_colour_nearblack.x;
    col.y = nov_colour_nearblack.y;
    col.z = nov_colour_nearblack.z;

    // fill box with black
    uint32_t top_left = (panel_origin.v * framebuffer.size.u) + panel_origin.u;
    for (uint32_t y = 0; y < panel_size.v; y++)
    {
        for (uint32_t x = 0; x < panel_size.u; x++)
        {
            framebuffer.address[(top_left * 3) + 0] = col.z;
            framebuffer.address[(top_left * 3) + 1] = col.y;
            framebuffer.address[(top_left * 3) + 2] = col.x;

            top_left++;
        }
        top_left = (top_left - panel_size.u) + framebuffer.size.u;
    }

    col.x = nov_colour_gold.x;
    col.y = nov_colour_gold.y;
    col.z = nov_colour_gold.z;

    // draw a box outline of size starting at origin

    top_left = ((panel_origin.v + 1) * framebuffer.size.u) + (panel_origin.u + 1);
    uint32_t top_right = top_left + (panel_size.u - 3);
    uint32_t bottom_left = top_left;
    uint32_t bottom_right = top_right;

    for (uint32_t t = 0; t < panel_size.v - 2; t++)
    {
        framebuffer.address[(bottom_left * 3) + 0] = col.z;
        framebuffer.address[(bottom_left * 3) + 1] = col.y;
        framebuffer.address[(bottom_left * 3) + 2] = col.x;

        framebuffer.address[(bottom_right * 3) + 0] = col.z;
        framebuffer.address[(bottom_right * 3) + 1] = col.y;
        framebuffer.address[(bottom_right * 3) + 2] = col.x;

        bottom_left += framebuffer.size.u;
        bottom_right += framebuffer.size.u;
    }

    top_right = top_left;
    bottom_left -= framebuffer.size.u;

    for (uint32_t t = 0; t < panel_size.u - 2; t++)
    {
        framebuffer.address[(top_right * 3) + 0] = col.z;
        framebuffer.address[(top_right * 3) + 1] = col.y;
        framebuffer.address[(top_right * 3) + 2] = col.x;

        framebuffer.address[(bottom_left * 3) + 0] = col.z;
        framebuffer.address[(bottom_left * 3) + 1] = col.y;
        framebuffer.address[(bottom_left * 3) + 2] = col.x;

        top_right++;
        bottom_left++;
    }
}

nov_boxed_text::nov_boxed_text(nov_framebuffer& _framebuffer, nov_ivector2& origin, nov_uvector2& size)
{
    framebuffer = _framebuffer;
    panel_origin = origin;
    panel_size = size;
}

void nov_boxed_text::set_position(nov_ivector2 &new_position, bool local)
{
    panel_origin = new_position;
}

}
}