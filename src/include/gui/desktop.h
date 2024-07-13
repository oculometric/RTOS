#pragma once

#include <gui/compositor.h>
#include <stdint.h>
#include <linked_list.h>

namespace nov
{
namespace gui
{

class SegmentedDesktop
{
private:
    Compositor* primary_compositor;

    LinkedList<ContainerHandle> handle_order;

    inline ContainerHandle getActiveHandle() { return handle_order[0]; }
public:
    SegmentedDesktop(uint8_t* framebuffer_address, uint16_t width, uint16_t height);
    SegmentedDesktop() = delete;

    void takeControl();

    ~SegmentedDesktop();
};

}
}