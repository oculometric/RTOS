#include <gui/compositor.h>
#include <serial.h>

// TODO: comments in this file

namespace nov
{
namespace gui
{

static uint32_t handle_next = 0xAC;

uint32_t Compositor::getUnusedHandleID()
{
    return handle_next++;
}

void Compositor::initContainerTree()
{
    serial::com_1 << "initialising container tree" << stream::endl;
    root = new Container();
    handle_map.push(ContainerLinkNode{ getUnusedHandleID(), root });
    root->handle_id = handle_map[0].handle_id;
    ContainerHandle handle = ContainerHandle(handle_map[0].handle_id, this);
    handle.compositor = this;
    UVector2 bounds = computeBounds(root, true).b;
    root->framebuffer_details = ProtectedFramebuffer(new uint8_t[3 * bounds.u * bounds.v], bounds, 3);
    root_handle = handle;
}

Pair<UVector2, UVector2> Compositor::computeBounds(Container* target, bool account_for_border)
{
    vector::UVector2 offset{ 0,0 };
    vector::UVector2 size = front_buffer.size;

    Container* walkup = target->parent;
    Container* child = target;
    while (walkup != nullptr)
    {
        if (walkup->division.u > 0)
        {
            if (child == walkup->child_a)
            {
                offset.u = offset.u * walkup->division.u;
                size.u *= walkup->division.u;
            }
            else if (child == walkup->child_b)
            {
                offset.u = (offset.u * (1 - walkup->division.u)) + (walkup->division.u * front_buffer.size.u);
                size.u *= 1 - walkup->division.u;
            }
            else panic ("OH FUCK");
        }
        else
        {
            if (child == walkup->child_a)
            {
                offset.v = offset.v * walkup->division.v;
                size.v *= walkup->division.v;
            }
            else if (child == walkup->child_b)
            {
                offset.v = (offset.v * (1 - walkup->division.v)) + (walkup->division.v * front_buffer.size.v);
                size.v *= 1 - walkup->division.v;
            }
            else panic("OH FUCK");
        }
        child = walkup;
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
    
    for (uint32_t i = 0; i < handle_map.getLength(); i++)
    {
        ContainerLinkNode node = handle_map[i];
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

void Compositor::blit(Container* target, ProtectedFramebuffer source)
{
    if (target == nullptr) return;
    if (target->parent == nullptr && target != root) return;
    if (target->child_a != nullptr || target->child_b != nullptr) return;
    if (front_buffer.address == nullptr) return;

    Pair<UVector2, UVector2> bounds = computeBounds(target, true);

    UVector2 src_off = UVector2{ 0,0 };
    UVector2 src_size = UVector2{ min(source.size.u, bounds.b.u), min(source.size.v, bounds.b.v) };

    graphics::copyBox(bounds.a, src_off, src_size, front_buffer, source.getWeakCopy());
}

void Compositor::clear(Container* target)
{
    if (target == nullptr) { serial::com_1 << "compositor clear target was null" << stream::endl; return; }
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

ProtectedFramebuffer* Compositor::getContainerFramebuffer(ContainerHandle handle)
{
    return &(findContainer(handle)->framebuffer_details);
}

void Compositor::blitContainer(ContainerHandle handle)
{
    Container* container = findContainer(handle);
    blit(container, container->framebuffer_details);
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
    if (!handle.isValid()) { serial::com_1 << "uh oh! container is invalid!" << stream::endl; return ContainerHandle(0, nullptr); }
    Container* target = findContainer(handle);
    if (target == nullptr) { serial::com_1 << "uh oh! unable to find container in tree!" << stream::endl; return ContainerHandle(0, nullptr); }
    if (target->child_a != nullptr || target->child_b != nullptr) { serial::com_1 << "uh oh! this container already has children!" << stream::endl; return ContainerHandle(0, nullptr); }

    Container* new_parent = new Container();
    new_parent->handle_id = getUnusedHandleID();
    handle_map.push(ContainerLinkNode{ new_parent->handle_id, new_parent });
    Container* new_container = new Container();
    new_container->handle_id = getUnusedHandleID();

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
    ContainerHandle new_handle = ContainerHandle(new_container->handle_id, this);
    handle_map.push(ContainerLinkNode{ new_handle.handle_id, new_container });

    // update the framebuffer for the old container
    if (target->framebuffer_details.address)
    {
        delete[] target->framebuffer_details.address;
    }
    UVector2 existing_container_new_bounds = computeBounds(target, true).b;
    target->framebuffer_details.address = new uint8_t[3 * existing_container_new_bounds.u * existing_container_new_bounds.v];
    target->framebuffer_details.bytes_per_pixel = 3;
    target->framebuffer_details.size = existing_container_new_bounds;

    // update framebuffer for the new container
    UVector2 new_container_bounds = computeBounds(new_container, true).b;
    new_container->framebuffer_details.address = new uint8_t[3 * new_container_bounds.u * new_container_bounds.v];
    new_container->framebuffer_details.bytes_per_pixel = 3;
    new_container->framebuffer_details.size = new_container_bounds;

    return new_handle;
}

ContainerHandle Compositor::getRootContainer()
{
    return root_handle;
}

bool isLeaf(Container* container)
{
    return (container->child_a == nullptr);
}

Container* Compositor::findClosestLeafContainer(Container* within, vector::UVector2 to, Direction preferred_direction)
{
    Container* target = within;
    IVector2 to_i = IVector2{ (int32_t)to.u, (int32_t)to.v };
    
    while (!isLeaf(target))
    {
        // TODO: i can skip out a lot of maths here. just compute the target's bounds and do the extra work from there
        //auto bounds_p = computeBounds(target, false);
        Pair<vector::UVector2, vector::UVector2> bounds_a = computeBounds(target->child_a);
        Pair<vector::UVector2, vector::UVector2> bounds_b = computeBounds(target->child_b);
        // if (target->division.u > 0)
        // {
        //     bounds_a = Pair<vector::UVector2, vector::UVector2>{ bounds_p.a, vector::UVector2{ bounds_p.b.u * target->division.u, bounds_p.b.v } };
        //     bounds_b = Pair<vector::UVector2, vector::UVector2>{ bounds_p.a + bounds_a.b, vector::UVector2{ bounds_p.b.u * (1 - target->division.u), bounds_p.b.v } };
        // }
        // else
        // {
        //     bounds_a = Pair<vector::UVector2, vector::UVector2>{ bounds_p.a, vector::UVector2{ bounds_p.b.u, bounds_p.b.v * target->division.v } };
        //     bounds_b = Pair<vector::UVector2, vector::UVector2>{ bounds_p.a + bounds_a.b, vector::UVector2{ bounds_p.b.u, bounds_p.b.v * (1 - target->division.v) } };
        // }

        vector::UVector2 center_a = bounds_a.a + (bounds_a.b / (uint32_t)2);
        vector::UVector2 center_b = bounds_b.a + (bounds_b.b / (uint32_t)2);

        int32_t dist_a = magSq(to_i - IVector2{ (int32_t)center_a.u, (int32_t)center_a.v });
        int32_t dist_b = magSq(to_i - IVector2{ (int32_t)center_b.u, (int32_t)center_b.v });

        if (dist_b < dist_a)
        {
            target = target->child_b;
        }
        else if (dist_a < dist_b)
        {
            target = target->child_a;
        }
        else
        {
            if (isLeaf(target->child_a) && !isLeaf(target->child_b))
                target = target->child_b;
            else
                target = target->child_a;
        }
    }

    return target;
}

ContainerHandle Compositor::getNorthContainer(ContainerHandle handle)
{
    Container* container = findContainer(handle);
    auto bounds = computeBounds(container, false);

    Container* parent = container->parent;

    while (parent != nullptr)
    {
        if (parent->division.v > 0 && parent->child_b == container)
            return ContainerHandle(findClosestLeafContainer(parent->child_a, bounds.a + (bounds.b / (uint32_t)2), Direction::SOUTH)->handle_id, this);

        container = parent;
        parent = parent->parent;
    }

    return handle;
}

ContainerHandle Compositor::getSouthContainer(ContainerHandle handle)
{
    Container* container = findContainer(handle);
    auto bounds = computeBounds(container, false);
    
    Container* parent = container->parent;

    while (parent != nullptr)
    {
        if (parent->division.v > 0 && parent->child_a == container)
            return ContainerHandle(findClosestLeafContainer(parent->child_b, bounds.a + (bounds.b / (uint32_t)2), Direction::NORTH)->handle_id, this);

        container = parent;
        parent = parent->parent;
    }

    return handle;
}

ContainerHandle Compositor::getWestContainer(ContainerHandle handle)
{
    Container* container = findContainer(handle);
    auto bounds = computeBounds(container, false);
    
    Container* parent = container->parent;

    while (parent != nullptr)
    {
        if (parent->division.u > 0 && parent->child_b == container)
            return ContainerHandle(findClosestLeafContainer(parent->child_a, bounds.a + (bounds.b / (uint32_t)2), Direction::EAST)->handle_id, this);

        container = parent;
        parent = parent->parent;
    }

    return handle;
}

ContainerHandle Compositor::getEastContainer(ContainerHandle handle)
{
    Container* container = findContainer(handle);
    auto bounds = computeBounds(container, false);

    Container* parent = container->parent;

    while (parent != nullptr)
    {
        if (parent->division.u > 0 && parent->child_a == container)
            return ContainerHandle(findClosestLeafContainer(parent->child_b, bounds.a + (bounds.b / (uint32_t)2), Direction::WEST)->handle_id, this);

        container = parent;
        parent = parent->parent;
    }

    return handle;
}

void dbg(Container* c, int d)
{
    for (int i = 0; i < d; i++)
        serial::com_1 << "    ";
    serial::com_1 << stream::HEX << (uint32_t)c << stream::endl;
    if (c)
    {
        dbg(c->child_a, d+1);
        dbg(c->child_b, d+1);
    }
}

void Compositor::debug()
{
    serial::com_1 << "compositor:" << stream::endl;
    serial::com_1 << "    map: " << stream::DEC << handle_map.getLength() << stream::endl;
    for (uint32_t i = 0; i < handle_map.getLength(); i++)
    {
        ContainerLinkNode node = handle_map[i];
        serial::com_1 << "        " << stream::HEX << node.handle_id << " : " << (uint32_t)node.container << stream::endl;
    }
    serial::com_1 << "" << stream::endl;

    serial::com_1 << "    root: " << (uint32_t)root << stream::endl;
    serial::com_1 << "    root handle: " << root_handle.handle_id << stream::endl;
    
    serial::com_1 << "    tree:" << stream::endl;
    
    dbg(root, 2);
}

ProtectedFramebuffer* ContainerHandle::getFramebuffer()
{
    if (compositor && isValid())
        return compositor->getContainerFramebuffer(*this);
    
    return nullptr;
}

void ContainerHandle::blit()
{
    if (compositor && isValid())
        compositor->blitContainer(*this);
}


void ContainerHandle::clear()
{
    if (compositor && isValid())
        compositor->clearContainer(*this);
}


void ContainerHandle::setTitle(String new_title)
{
    if (compositor && isValid())
        compositor->setContainerTitle(*this, new_title);
}

void ContainerHandle::setDecoration(ContainerDecorationMode mode)
{
    if (compositor && isValid())
        compositor->setContainerDecoration(*this, mode);
}

}
}