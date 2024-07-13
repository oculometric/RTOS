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
    Compositor* primary_compositor = nullptr;

    LinkedList<ContainerHandle> handle_order;
    bool is_overlay_showing = false;
    Font* font = nullptr;

    inline ContainerHandle getActiveHandle() { return handle_order[0]; }
public:
    SegmentedDesktop(uint8_t* framebuffer_address, uint16_t width, uint16_t height);
    SegmentedDesktop() = delete;

    void takeControl();

    ~SegmentedDesktop();
};

}
}