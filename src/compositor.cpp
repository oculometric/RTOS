#include <gui/compositor.h>

namespace nov
{
namespace gui
{

uint32_t handle_next = 0xAC;

uint32_t Compositor::getUnusedHandleID()
{
    return handle_next++;
}

void Compositor::initContainerTree()
{
    root = new Container();
    handle_map.push(ContainerLinkNode{ getUnusedHandleID(), root });
}

Compositor::Compositor(uint8_t* framebuffer_address, uint16_t width, uint16_t height, Font* font)
{
    initContainerTree();
    front_buffer.bytes_per_pixel = 3;
    front_buffer.address = framebuffer_address;
    guiFont = font;

    resizeCanvas(width, height);
    clear(root);
}

void Compositor::clear(Container* target)
{
    if (target == nullptr) return;
    if (target->parent == nullptr && target != root) return;
    if (target->child_a != nullptr || target->child_b != nullptr) return;
    if (front_buffer.address == nullptr) return;
    
    // compute size of the frame
    vector::UVector2 offset{ 0,0 };
    vector::UVector2 size = front_buffer.size;

    Container* walkup = target->parent;
    while (walkup != nullptr)
    {
        if (walkup->division.u > 0)
        {
            if (target == walkup->child_a)
                size.u *= walkup->division.u;
            else
            {
                offset.u = (offset.u * (1 - walkup->division.u)) + (walkup->division.u * front_buffer.size.u);
                size.u *= 1 - walkup->division.u;
            }
        }
        else
        {
            if (target == walkup->child_a)
                size.v *= walkup->division.v;
            else
            {
                offset.v = (offset.v * (1 - walkup->division.v)) + (walkup->division.v * front_buffer.size.v);
                size.v *= 1 - walkup->division.v;
            }
        }
        walkup = walkup->parent;
    }

    // TODO: better bordering
    graphics::fillBox(offset, size, target->clear_colour, front_buffer);

    if (target->decoration_mode == ContainerDecorationMode::BORDERLESS) return;

    graphics::drawBox(offset + UVector2{ 1,1 }, size - UVector2{ 2,2 }, target->frame_colour, front_buffer);

    if (target->decoration_mode == ContainerDecorationMode::BORDERED) return;
    graphics::fillBox(offset + UVector2{ 2,2 }, UVector2{ size.u - 4,10 }, target->frame_colour, front_buffer);

    uint32_t length = target->title.getLength();
    UVector2 character_origin = offset + UVector2{8,1};
    for (uint32_t char_ind = 0; char_ind < length; char_ind++)
    {
        char chr = target->title[char_ind];
        graphics::drawCharacter(chr, character_origin, nov_colour_black, *guiFont, front_buffer);
        character_origin.u += guiFont->char_width + 1;
    }
}

void Compositor::resizeCanvas(uint16_t width, uint16_t height)
{
    front_buffer.size = vector::UVector2{ width, height };

    // TODO: resize children oh god
}

}
}