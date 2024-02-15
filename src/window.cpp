#include <window.h>

#include <serial.h>
#include <memory.h>
#include <array.h>
#include <graphics.h>

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

// void nov_boxed_text::draw()
// {
//     if (panel_size.u < 3 || panel_size.v < 3) return;

//     nov_vector3<uint8_t> col;
//     col.x = nov_colour_nearblack.x;
//     col.y = nov_colour_nearblack.y;
//     col.z = nov_colour_nearblack.z;

//     // fill box with black
//     uint32_t top_left = (panel_origin.v * framebuffer.size.u) + panel_origin.u;
//     for (uint32_t y = 0; y < panel_size.v; y++)
//     {
//         for (uint32_t x = 0; x < panel_size.u; x++)
//         {
//             framebuffer.address[(top_left * 3) + 0] = col.z;
//             framebuffer.address[(top_left * 3) + 1] = col.y;
//             framebuffer.address[(top_left * 3) + 2] = col.x;

//             top_left++;
//         }
//         top_left = (top_left - panel_size.u) + framebuffer.size.u;
//     }

//     col.x = nov_colour_gold.x;
//     col.y = nov_colour_gold.y;
//     col.z = nov_colour_gold.z;

//     // draw a box outline of size starting at origin

//     top_left = ((panel_origin.v + 1) * framebuffer.size.u) + (panel_origin.u + 1);
//     uint32_t top_right = top_left + (panel_size.u - 3);
//     uint32_t bottom_left = top_left;
//     uint32_t bottom_right = top_right;

//     for (uint32_t t = 0; t < panel_size.v - 2; t++)
//     {
//         framebuffer.address[(bottom_left * 3) + 0] = col.z;
//         framebuffer.address[(bottom_left * 3) + 1] = col.y;
//         framebuffer.address[(bottom_left * 3) + 2] = col.x;

//         framebuffer.address[(bottom_right * 3) + 0] = col.z;
//         framebuffer.address[(bottom_right * 3) + 1] = col.y;
//         framebuffer.address[(bottom_right * 3) + 2] = col.x;

//         bottom_left += framebuffer.size.u;
//         bottom_right += framebuffer.size.u;
//     }

//     top_right = top_left;
//     bottom_left -= framebuffer.size.u;

//     for (uint32_t t = 0; t < panel_size.u - 2; t++)
//     {
//         framebuffer.address[(top_right * 3) + 0] = col.z;
//         framebuffer.address[(top_right * 3) + 1] = col.y;
//         framebuffer.address[(top_right * 3) + 2] = col.x;

//         framebuffer.address[(bottom_left * 3) + 0] = col.z;
//         framebuffer.address[(bottom_left * 3) + 1] = col.y;
//         framebuffer.address[(bottom_left * 3) + 2] = col.x;

//         top_right++;
//         bottom_left++;
//     }
// }

// nov_boxed_text::nov_boxed_text(nov_framebuffer& _framebuffer, nov_ivector2& origin, nov_uvector2& size)
// {
//     framebuffer = _framebuffer;
//     panel_origin = origin;
//     panel_size = size;
// }

// void nov_boxed_text::set_position(nov_ivector2 &new_position, bool local)
// {
//     panel_origin = new_position;
// }

void nov_panel::draw(nov_frame_data* frame, const graphics::nov_framebuffer& framebuffer)
{
    // TODO: virtual?
}

void calculate_frame_data(nov_frame_data* parent, const nov_fvector2& division, nov_frame_data* frame_a, nov_frame_data* frame_b)
{
    // calculate the halfway point between child frames
    nov_uvector2 halfway;
    if (division.u == 0) halfway = nov_uvector2{ parent->size.u, (uint32_t)(((float)parent->size.v) * division.v) };
    else halfway = nov_uvector2{ (uint32_t)(((float)parent->size.u) * division.u), parent->size.v };

    // if frame_a is not null, populate it
    if (frame_a != 0x0)
    {
        frame_a->origin = parent->origin;
        frame_a->size = halfway;
    }
    
    if (frame_b != 0x0)
    {
        if (division.u == 0)
        {
            frame_b->origin = nov_uvector2{ parent->origin.u, parent->origin.v + halfway.v };
            frame_b->size = nov_uvector2{ halfway.u, parent->size.v - halfway.v };
        }
        else
        {
            frame_b->origin = nov_uvector2{ parent->origin.u + halfway.u, parent->origin.v };
            frame_b->size = nov_uvector2{ parent->size.u - halfway.u, halfway.v };
        }
    }
}

void split_container(nov_container* parent, const nov_fvector2& division)
{
    // parent cannot be null
    if (parent == 0x0) return;
    // one of the divisors must be zero
    if (division.u != 0 && division.v != 0) return;

    // create children
    parent->child_a = new nov_container { 0x0, 0x0, parent->panel, parent, nov_fvector2{ 0,0 } };
    parent->child_b = new nov_container { 0x0, 0x0, 0x0, parent, nov_fvector2{ 0,0 } };

    // clear parent panel
    parent->panel = 0x0;

    // set parent division
    parent->division = division;
}

// TODO: add limits to prevent negative overflows
void nov_gui_manager::draw_container(nov_container* container, nov_frame_data* frame)
{
    // fill with black
    graphics::fill_box(frame->origin, frame->size, frame_fill_colour, framebuffer);

    if (container->panel != 0x0 || (container->child_a == 0x0 && container->child_b == 0x0))
    {
        // TODO: text label/title
        // draw the top bar
        graphics::fill_box(frame->origin+nov_uvector2{1,1}, nov_uvector2{frame->size.u-2,10}, frame_outline_colour, framebuffer);
        // coloured outline
        graphics::draw_box(frame->origin+nov_uvector2{1,1}, frame->size-nov_uvector2{2,2}, frame_outline_colour, framebuffer);
    }

    // draw panel, if it exists
    if (container->panel != 0x0)
    {
        // TODO: reduce frame size before passing to panel
        container->panel->draw(frame, framebuffer);
    }
    else
    {
        // otherwise try to draw children
        // if neither exist, skip
        if (container->child_a == 0x0 && container->child_b == 0x0) return;

        // otherwise calculate their frames
        nov_frame_data frame_a;
        nov_frame_data frame_b;
        calculate_frame_data(frame, container->division, &frame_a, &frame_b);

        if (container->child_a != 0x0) draw_container(container->child_a, &frame_a);
        if (container->child_b != 0x0) draw_container(container->child_b, &frame_b);
    }
}

bool nov_gui_manager::draw_specific(nov_container* container)
{ // FIXME: this is not working correctly
    // return if this container is invalid
    if (container == 0x0) return false;

    // return if this container has no parent and is not the root container
    if (container->parent == 0x0 && container != root_container) return false;

    nov_array<nov_container*> container_chain;
    nov_container* current = container;

    serial_print((char*)"finding path to ", COM1); serial_println_hex((uint32_t)container, COM1);

    // now we need to find this container in the tree
    while (current != root_container && current->parent != 0x0)
    {
        serial_print((char*)"pushing current to chain: ", COM1); serial_println_hex((uint32_t)current, COM1);
        container_chain.push(current);
        current = current->parent;
        serial_print((char*)"parent of current: ", COM1); serial_println_hex((uint32_t)current, COM1);
    } 

    serial_println((char*)"ok, we have the chain.", COM1);
    serial_println_dec(container_chain.get_length(), COM1);
    
    // if we had to break before finding the root, return
    if (current != root_container) return false;

    // otherwise, we ended up with a chain of panels from the root to the desired container
    nov_frame_data current_frame_data = root_container_frame;
    nov_frame_data next_frame_data;

    nov_container* next = 0x0;
    serial_println((char*)"walking down the chain: ", COM1);
    while (container_chain.get_length() > 0)
    {
        // get the next container in the chain
        next = container_chain.pop();
        serial_println_hex((uint32_t)current, COM1);
        // calculate the frame data, choosing which child we want based on whether the next container is the a or b child
        calculate_frame_data(&current_frame_data, current->division, next == current->child_a ? &next_frame_data : 0x0, next == current->child_b ? &next_frame_data : 0x0);
        // copy the data from this container into the current one
        current_frame_data = next_frame_data;
        current = next;
    }

    serial_println((char*)"all done.", COM1);

    // now, draw current container and all children
    draw_container(current, &current_frame_data);

    return true;
}

nov_gui_manager::nov_gui_manager(const graphics::nov_framebuffer& framebuffer_info, nov_container* default_root)
{
    root_container_frame.origin = nov_uvector2{ 0,0 };
    root_container_frame.size = framebuffer_info.size;
    
    framebuffer = framebuffer_info;

    if (default_root == 0x0)
        root_container = new nov_container{ 0x0, 0x0, 0x0, 0x0, nov_fvector2{ 0,0 } };
    else
        root_container = default_root;
}

}
}