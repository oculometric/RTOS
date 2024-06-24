#pragma once

#include <stdint.h>

#include <graphics.h>

namespace nov
{
namespace gui
{

enum ContainerDecorationMode
{
    BORDERED,
    BORDERED_WITH_TITLE,
    BORDERLESS
};

class ContainerHandle
{
private:
    uint32_t handle_id = 0;
    uint16_t subdivision_address = 0;
    bool is_valid = false;
    graphics::Framebuffer framebuffer_details;

    ContainerHandle();
public:
    ContainerHandle(ContainerHandle& other) = delete;
    ContainerHandle(ContainerHandle&& other) = delete;

    inline bool isValid() { return is_valid; }
    inline graphics::Framebuffer getFramebuffer() { return framebuffer_details; }
    void blit();
    void setDecoration(ContainerDecorationMode mode);

    friend class Compositor;
};

enum ContainerSplitDecision
{
    SPLIT_VERTICAL_LEFT,
    SPLIT_VERTICAL_RIGHT,
    SPLIT_HORIZONTAL_TOP,
    SPLIT_HORIZONTAL_BOTTOM
};

class Compositor
{
private:

public:
    Compositor();

    void resizeCanvas(uint32_t width, uint32_t height);
    ContainerHandle createContainer(ContainerHandle existing_container, ContainerSplitDecision split);
    void destroyContainer();

    Compositor(Compositor& other) = delete;
    Compositor(Compositor&& other) = delete;
    Compositor& operator=(const Compositor& other) = delete;
    Compositor& operator=(Compositor&& other) = delete;

    friend class ContainerHandle;
};

}
}