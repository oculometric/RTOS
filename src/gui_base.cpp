#include <gui/gui_base.h>

#include <array.h>
#include <colour.h>
#include <graphics.h>

namespace nov
{
namespace gui
{

void calculateFrameData(const FrameData& parent, const FVector2& division, FrameData* frame_a, FrameData* frame_b)
{
    // calculate the halfway point between child frames
    UVector2 halfway;
    if (division.u == 0) halfway = UVector2{ parent.size.u, (uint32_t)(((float)parent.size.v) * division.v) };
    else halfway = UVector2{ (uint32_t)(((float)parent.size.u) * division.u), parent.size.v };

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
            frame_b->origin = UVector2{ parent.origin.u, parent.origin.v + halfway.v };
            frame_b->size = UVector2{ halfway.u, parent.size.v - halfway.v };
        }
        else
        {
            frame_b->origin = UVector2{ parent.origin.u + halfway.u, parent.origin.v };
            frame_b->size = UVector2{ parent.size.u - halfway.u, halfway.v };
        }
    }
}

void splitContainer(Container* parent, const FVector2& division)
{
    // parent cannot be null
    if (parent == 0x0) return;
    // one of the divisors must be zero
    if (division.u != 0 && division.v != 0) return;

    // create children
    parent->child_a = new Container{ 0x0, 0x0, parent->panel, parent, FVector2{ 0,0 } };
    parent->child_b = new Container{ 0x0, 0x0, 0x0, parent, FVector2{ 0,0 } };

    // clear parent panel
    parent->panel = 0x0;

    // set parent division
    parent->division = division;
}

void GuiManager::drawContainer(Container* container, const FrameData& frame)
{
    com_1 << "drawing container" << (uint32_t)container << stream::endl;
    
    // fill with black
    if ((container->panel != 0x0 && container->panel->wantsClear()) || container->panel == 0x0)
    {
        graphics::fillBox(frame.origin, frame.size, frame_fill_colour, framebuffer);
    }

    bool large_enough_for_frame = (frame.size.u >= 8 && frame.size.v > 12);

    if (large_enough_for_frame
        && ((container->panel != 0x0 && container->panel->wantsBorder())
         || (container->panel == 0x0 && container->child_a == 0x0 && container->child_b == 0x0))
       )
    {
        // draw the top bar
        graphics::fillBox(frame.origin+UVector2{1,1}, UVector2{frame.size.u-2,10}, frame_outline_colour, framebuffer);
        // coloured outline
        graphics::drawBox(frame.origin+UVector2{1,1}, frame.size-UVector2{2,2}, frame_outline_colour, framebuffer);
        // draw title
        if (guiFont)
        {
            uint32_t length = container->panel->name.getLength();
            UVector2 character_origin = frame.origin + UVector2{8,1};
            for (uint32_t char_ind = 0; char_ind < length; char_ind++)
            {
                char chr = container->panel->name[char_ind];
                graphics::drawCharacter(chr, character_origin, nov_colour_black, *guiFont, framebuffer);
                character_origin.u += guiFont->char_width + 1;
            }
        }
    }

    // draw panel, if it exists
    if (container->panel != 0x0)
    {
        com_1 << "panel named " << container->panel->name << stream::endl;
        FrameData clipped = frame;
        if (container->panel->wantsBorder())
        {
            if (!large_enough_for_frame) return;
            clipped.origin += UVector2{ 3,12 };
            clipped.size -= UVector2{ 6,15 };
        }
        else
        {
            clipped.origin += UVector2{ 1,1 };
            clipped.size -= UVector2{ 2,2 };
        }
        container->panel->draw(clipped, framebuffer);
    }
    else
    {
        // otherwise try to draw children
        // if neither exist, skip
        if (container->child_a == 0x0 && container->child_b == 0x0) return;

        // otherwise calculate their frames
        FrameData frame_a;
        FrameData frame_b;
        calculateFrameData(frame, container->division, &frame_a, &frame_b);

        if (container->child_a != 0x0) drawContainer(container->child_a, frame_a);
        if (container->child_b != 0x0) drawContainer(container->child_b, frame_b);
    }
}

bool GuiManager::drawSpecific(Container* container)
{
    // return if this container is invalid
    if (container == 0x0) return false;

    // first, check if this container has been cached. if it has, then use the cached data, otherwise
    // calculate it from scratch
    for (FrameCache fc : frame_cache)
    {
        if (fc.container == container)
        {
            drawContainer(container, fc.frame_data);
            return true;
        }
    }

    com_1 << "when drawing frame " << stream::Mode::HEX << (uint32_t)container << " found no cached data." << stream::endl;

    // return if this container has no parent and is not the root container
    if (container->parent == 0x0 && container != root_container) return false;

    Array<Container*> container_chain;
    Container* current = container;

    // now we need to find this container in the tree
    while (current != root_container && current->parent != 0x0)
    {
        container_chain.push(current);
        current = current->parent;
    } 

    // if we had to break before finding the root, return
    if (current != root_container) return false;

    // otherwise, we ended up with a chain of panels from the root to the desired container
    FrameData current_frame_data = root_container_frame;
    FrameData next_frame_data;

    Container* next = 0x0;
    while (container_chain.getLength() > 0)
    {
        // get the next container in the chain
        next = container_chain.pop();
        // calculate the frame data, choosing which child we want based on whether the next container is the a or b child
        calculateFrameData(current_frame_data, current->division, next == current->child_a ? &next_frame_data : 0x0, next == current->child_b ? &next_frame_data : 0x0);
        // copy the data from this container into the current one
        current_frame_data = next_frame_data;
        current = next;
    }

    // now, draw current container and all children
    drawContainer(current, current_frame_data);
    frame_cache.push(FrameCache{ current_frame_data, container });

    return true;
}

GuiManager::GuiManager(const graphics::Framebuffer& framebuffer_info, Container* default_root)
{
    root_container_frame.origin = UVector2{ 0,0 };
    root_container_frame.size = framebuffer_info.size;
    
    framebuffer = framebuffer_info;

    if (default_root == 0x0)
        root_container = new Container{ 0x0, 0x0, 0x0, 0x0, FVector2{ 0,0 } };
    else
        root_container = default_root;
}

}
}