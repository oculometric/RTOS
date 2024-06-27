#include <gui/compositor.h>
#include <serial.h>

namespace nov
{
namespace gui
{

static uint32_t handle_next = 0xAC;

uint32_t Compositor::getUnusedHandleID()
{
    return handle_next++;
}

// TODO: move framebuffer details into compositor so they can be got through the containerhandle

void Compositor::initContainerTree()
{
    serial::com_1 << "initialising container tree" << stream::endl;
    root = new Container();
    handle_map.push(ContainerLinkNode{ getUnusedHandleID(), root });
    root_handle = ContainerHandle(handle_map[0].handle_id);
    ContainerHandle handle = ContainerHandle(handle_map[0].handle_id);
    handle.compositor = this;
    UVector2 bounds = computeBounds(root, true).b;
    handle.framebuffer_details = new graphics::Framebuffer{ new uint8_t[3 * bounds.u * bounds.v], bounds, 3 };
    serial::com_1 << "framebuffer address: " << (uint32_t)handle.framebuffer_details << stream::endl;
    serial::com_1 << stream::DEC << "bounds: " << bounds.u << ", " << bounds.v << stream::endl;
    root_handle = handle;
}

Pair<UVector2, UVector2> Compositor::computeBounds(Container* target, bool account_for_border)
{
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

    if (account_for_border)
    {
        switch (target->decoration_mode)
        {
        case ContainerDecorationMode::BORDERED_WITH_TITLE:
            offset += UVector2{ 3, 13 };
            size -= UVector2{ 6, 16 };
            break;
        case ContainerDecorationMode::BORDERED:
            offset += UVector2{ 3, 3 };
            size -= UVector2{ 6, 6 };
            break;
        case ContainerDecorationMode::BORDERLESS:
            break;
        }
    }

    return Pair<UVector2, UVector2>{ offset, size };
}

Container* Compositor::findContainer(ContainerHandle handle)
{
    if (!handle.isValid()) return nullptr;
    
    for (ContainerLinkNode node : handle_map)
    {
        if (node.handle_id == handle.handle_id) return node.container;
    }

    return nullptr;
}

Compositor::Compositor(uint8_t* framebuffer_address, uint16_t width, uint16_t height, Font *font)
{
    front_buffer.size = UVector2{ width, height };
    initContainerTree();
    front_buffer.bytes_per_pixel = 3;
    front_buffer.address = framebuffer_address;
    guiFont = font;

    resizeCanvas(width, height);
    clear(root);
}

void Compositor::blit(Container* target, graphics::Framebuffer source)
{
    if (target == nullptr) return;
    if (target->parent == nullptr && target != root) return;
    if (target->child_a != nullptr || target->child_b != nullptr) return;
    if (front_buffer.address == nullptr) return;

    Pair<UVector2, UVector2> bounds = computeBounds(target, true);

    UVector2 src_off = UVector2{ 0,0 };
    UVector2 src_size = UVector2{ min(source.size.u, bounds.b.u), min(source.size.v, bounds.b.v) };

    graphics::copyBox(bounds.a, src_off, src_size, front_buffer, source);
}

void Compositor::clear(Container* target)
{
    if (target == nullptr) return;
    if (target->parent == nullptr && target != root) return;
    if (target->child_a != nullptr || target->child_b != nullptr) return;
    if (front_buffer.address == nullptr) return;
    
    // compute size of the frame
    Pair<UVector2, UVector2> bounds = computeBounds(target);
    UVector2 offset = bounds.a;
    UVector2 size = bounds.b;
    graphics::fillBox(offset, size, target->clear_colour, front_buffer);

    repaint(target);
}

void Compositor::repaint(Container* target)
{
    // TODO: better bordering

    Pair<UVector2, UVector2> bounds = computeBounds(target);
    UVector2 offset = bounds.a;
    UVector2 size = bounds.b;

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

void Compositor::blitContainer(ContainerHandle handle)
{
    blit(findContainer(handle), handle.getFramebuffer());
}

void Compositor::clearContainer(ContainerHandle handle)
{
    clear(findContainer(handle));
}

void Compositor::setContainerDecoration(ContainerHandle handle, ContainerDecorationMode mode)
{
    Container* container = findContainer(handle);
    if (container != nullptr)
    {
        container->decoration_mode = mode;
        repaint(container);
    }
}

void Compositor::setContainerTitle(ContainerHandle handle, String new_title)
{
    Container* container = findContainer(handle);
    if (container != nullptr)
    {
        container->title = new_title;
        repaint(container);
    }
}

void Compositor::resizeCanvas(uint16_t width, uint16_t height)
{
    front_buffer.size = vector::UVector2{ width, height };

    // TODO: resize children oh god
}

ContainerHandle Compositor::divideContainer(ContainerHandle handle, ContainerSplitDecision split)
{
    if (!handle.isValid()) return ContainerHandle(0);
    Container* target = findContainer(handle);
    if (target == nullptr) return ContainerHandle(0);
    if (target->child_a != nullptr || target->child_b != nullptr) return ContainerHandle(0);

    Container* new_parent = new Container();
    handle_map.push(ContainerLinkNode{ getUnusedHandleID(), new_parent });
    Container* new_container = new Container();

    // make a new parent container and replace the target with it
    if (target != root)
    {
        if (target->parent->child_a == target)
            target->parent->child_a = new_parent;
        else
            target->parent->child_b = new_parent;
    }
    else
    {
        root = new_parent;
    }

    // add both the existing container and the new container as children of the new parent
    if (split == ContainerSplitDecision::SPLIT_HORIZONTAL_TOP || split == ContainerSplitDecision::SPLIT_VERTICAL_LEFT)
    {
        new_parent->child_b = target;
        new_parent->child_a = new_container;
    }
    else
    {
        new_parent->child_a = target;
        new_parent->child_b = new_container;
    }

    new_parent->parent = target->parent;
    new_container->parent = new_parent;
    target->parent = new_parent;
    
    // set the division of the new parent container
    if (split == ContainerSplitDecision::SPLIT_HORIZONTAL_BOTTOM || split == ContainerSplitDecision::SPLIT_HORIZONTAL_TOP)
        new_parent->division = FVector2{ 0, 0.5 };
    else
        new_parent->division = FVector2{ 0.5, 0 };

    // create a new container handle for the new container
    ContainerHandle new_handle = ContainerHandle(getUnusedHandleID());
    new_handle.compositor = this;
    handle_map.push(ContainerLinkNode{ new_handle.handle_id, new_container });

    // update the framebuffer for the old container
    if (handle.framebuffer_details->address)
    {
        delete[] handle.framebuffer_details->address;
    }
    UVector2 existing_container_new_bounds = computeBounds(target).b - UVector2{ 6,6 };
    handle.framebuffer_details->address = new uint8_t[3 * existing_container_new_bounds.u * existing_container_new_bounds.v];
    handle.framebuffer_details->bytes_per_pixel = 3;
    handle.framebuffer_details->size = existing_container_new_bounds;

    // update framebuffer for the new container
    UVector2 new_container_bounds = computeBounds(new_container).b - UVector2{ 6,6 };
    new_handle.framebuffer_details = new graphics::Framebuffer;
    new_handle.framebuffer_details->address = new uint8_t[3 * new_container_bounds.u * new_container_bounds.v];
    new_handle.framebuffer_details->bytes_per_pixel = 3;
    new_handle.framebuffer_details->size = new_container_bounds;

    return new_handle;
}

ContainerHandle Compositor::getRootContainer()
{
    return root_handle;
}

void Compositor::debug()
{
    serial::com_1 << "compositor:" << stream::endl;
    serial::com_1 << "    map: " << stream::DEC << handle_map.getLength() << stream::endl;
    for (int i = 0; i < handle_map.getLength(); i++)
    {
        ContainerLinkNode node = handle_map[i];
        serial::com_1 << "        " << stream::HEX << node.handle_id << " : " << (uint32_t)node.container << stream::endl;
    }
    serial::com_1 << "" << stream::endl;

}

void ContainerHandle::blit()
{
    compositor->blitContainer(*this);
}


void ContainerHandle::clear()
{
    compositor->clearContainer(*this);
}

}
}