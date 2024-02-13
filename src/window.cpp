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
    nov_uvector3 colour;
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

/**
 * causes immediate children to be redrawn, then calls redraw_children recursively on those children
 * 
 * **/
void nov_panel::redraw_children()
{
    for (uint32_t i = 0; i < children.get_length(); i++)
    {
        if (children[i] == 0x0) continue;
        children[i]->draw();
        children[i]->redraw_children();
    }
}

/**
 * recalculates the positions of immediate children, then recursively calls recalculate_child_positions
 * on thse children
 * 
 * @param preserve_global whether to preserve the global position of the children. if this is false,
 * then global positions are recalculated according to the parent's global position; if this is true,
 * then local positions are recalculated to fit with the parent's global position
 * 
 * **/
void nov_panel::recalculate_child_positions(bool preserve_global)
{
    for (uint32_t i = 0; i < children.get_length(); i++)
    {
        if (children[i] == 0x0) continue;
        if (preserve_global)
        {
            children[i]->panel_origin_local = children[i]->panel_origin - panel_origin;
        }
        else
        {
            children[i]->panel_origin = children[i]->panel_origin_local + panel_origin;
        }
        children[i]->recalculate_child_positions(preserve_global);
    }
}

}
}