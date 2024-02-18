#include <gui/gui.h>

#include <serial.h>
#include <memory.h>
#include <array.h>
#include <colour.h>
#include <graphics.h>

namespace nov
{
namespace gui
{

void calculate_frame_data(const nov_frame_data& parent, const nov_fvector2& division, nov_frame_data* frame_a, nov_frame_data* frame_b)
{
    // calculate the halfway point between child frames
    nov_uvector2 halfway;
    if (division.u == 0) halfway = nov_uvector2{ parent.size.u, (uint32_t)(((float)parent.size.v) * division.v) };
    else halfway = nov_uvector2{ (uint32_t)(((float)parent.size.u) * division.u), parent.size.v };

    // if frame_a is not null, populate it
    if (frame_a != 0x0)
    {
        frame_a->origin = parent.origin;
        frame_a->size = halfway;
    }
    
    if (frame_b != 0x0)
    {
        if (division.u == 0)
        {
            frame_b->origin = nov_uvector2{ parent.origin.u, parent.origin.v + halfway.v };
            frame_b->size = nov_uvector2{ halfway.u, parent.size.v - halfway.v };
        }
        else
        {
            frame_b->origin = nov_uvector2{ parent.origin.u + halfway.u, parent.origin.v };
            frame_b->size = nov_uvector2{ parent.size.u - halfway.u, halfway.v };
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

void nov_gui_manager::draw_container(nov_container* container, const nov_frame_data& frame)
{
    // fill with black
    if ((container->panel != 0x0 && container->panel->wants_clear()) || container->panel == 0x0)
    {
        graphics::fill_box(frame.origin, frame.size, frame_fill_colour, framebuffer);
    }

    bool large_enough_for_frame = (frame.size.u >= 8 && frame.size.v > 12);

    if (large_enough_for_frame
        && ((container->panel != 0x0 && container->panel->wants_border())
         || (container->child_a == 0x0 && container->child_b == 0x0))
       )
    {
        // TODO: text label/title
        // draw the top bar
        graphics::fill_box(frame.origin+nov_uvector2{1,1}, nov_uvector2{frame.size.u-2,10}, frame_outline_colour, framebuffer);
        // coloured outline
        graphics::draw_box(frame.origin+nov_uvector2{1,1}, frame.size-nov_uvector2{2,2}, frame_outline_colour, framebuffer);
    }

    // draw panel, if it exists
    if (container->panel != 0x0)
    {
        nov_frame_data clipped = frame;
        if (container->panel->wants_border())
        {
            if (!large_enough_for_frame) return;
            clipped.origin += nov_uvector2{ 3,11 };
            clipped.size -= nov_uvector2{ 6,14 };
        }
        else
        {
            clipped.origin += nov_uvector2{ 1,1 };
            clipped.size -= nov_uvector2{ 2,2 };
        }
        container->panel->draw(clipped, framebuffer);
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

        if (container->child_a != 0x0) draw_container(container->child_a, frame_a);
        if (container->child_b != 0x0) draw_container(container->child_b, frame_b);
    }
}

bool nov_gui_manager::draw_specific(nov_container* container)
{
    // return if this container is invalid
    if (container == 0x0) return false;

    // return if this container has no parent and is not the root container
    if (container->parent == 0x0 && container != root_container) return false;

    nov_array<nov_container*> container_chain;
    nov_container* current = container;

    // now we need to find this container in the tree
    while (current != root_container && current->parent != 0x0)
    {
        container_chain.push(current);
        current = current->parent;
    } 

    // if we had to break before finding the root, return
    if (current != root_container) return false;

    // otherwise, we ended up with a chain of panels from the root to the desired container
    nov_frame_data current_frame_data = root_container_frame;
    nov_frame_data next_frame_data;

    nov_container* next = 0x0;
    while (container_chain.get_length() > 0)
    {
        // get the next container in the chain
        next = container_chain.pop();
        // calculate the frame data, choosing which child we want based on whether the next container is the a or b child
        calculate_frame_data(current_frame_data, current->division, next == current->child_a ? &next_frame_data : 0x0, next == current->child_b ? &next_frame_data : 0x0);
        // copy the data from this container into the current one
        current_frame_data = next_frame_data;
        current = next;
    }

    // now, draw current container and all children
    draw_container(current, current_frame_data);

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