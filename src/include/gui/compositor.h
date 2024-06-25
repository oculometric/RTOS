#pragma once

#include <stdint.h>

#include <graphics.h>
#include <string.h>
#include <vector.h>
#include <array.h>
#include <colour.h>

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
    graphics::Framebuffer framebuffer_details;

    bool has_been_resized = false;

    ContainerHandle();
public:
    ContainerHandle(ContainerHandle& other) = delete;
    ContainerHandle(ContainerHandle&& other) = delete;

    inline bool isValid() { return handle_id != 0; }
    inline graphics::Framebuffer getFramebuffer() { return framebuffer_details; }
    void blit();
    void clear();
    void setDecoration(ContainerDecorationMode mode);
    void setTitle(String new_title);

    friend class Compositor;
};

enum ContainerSplitDecision
{
    SPLIT_VERTICAL_LEFT,
    SPLIT_VERTICAL_RIGHT,
    SPLIT_HORIZONTAL_TOP,
    SPLIT_HORIZONTAL_BOTTOM
};

struct Container
{
    Container* child_a = nullptr;
    Container* child_b = nullptr;

    Container* parent = nullptr;

    vector::FVector2 division;
    ContainerDecorationMode decoration_mode = ContainerDecorationMode::BORDERED_WITH_TITLE;
    String title = "container";
    Colour clear_colour = nov_colour_nearblack;
    Colour frame_colour = nov_colour_gold;
};

struct ContainerLinkNode
{
    uint32_t handle_id = 0;
    Container* container = nullptr;
};

class Compositor
{
private:
    Container* root = nullptr;
    graphics::Framebuffer front_buffer;
    uint8_t* framebuffer_address = nullptr;
    Font* guiFont = nullptr;

    Array<ContainerLinkNode> handle_map;
    uint32_t getUnusedHandleID();

    void blit(Container* target, graphics::Framebuffer source);
    void clear(Container* target);

    void blitContainer(ContainerHandle handle);
    void clearContainer(ContainerHandle handle);
    void setContainerDecoration(ContainerHandle handle, ContainerDecorationMode mode);
    void setContainerTitle(ContainerHandle handle, String new_title);

    void initContainerTree();
public:
    Compositor(uint8_t* framebuffer_address, uint16_t width, uint16_t height, Font* font);

    void resizeCanvas(uint16_t width, uint16_t height);
    //ContainerHandle createContainer(uint32_t existing_id = 0, ContainerSplitDecision split);
    ContainerHandle getRootContainer();
    void destroyContainer(ContainerHandle handle);

    Compositor() = delete;
    Compositor(Compositor& other) = delete;
    Compositor(Compositor&& other) = delete;
    Compositor& operator=(const Compositor& other) = delete;
    Compositor& operator=(Compositor&& other) = delete;

    friend class ContainerHandle;
};

}
}