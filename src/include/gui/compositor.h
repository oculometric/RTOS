#pragma once

#include <stdint.h>

#include <graphics.h>
#include <string.h>
#include <vector.h>
#include <array.h>
#include <colour.h>
#include <pair.h>

// TODO: comments in this file

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

class Compositor;

class ProtectedFramebuffer
{
private:
    uint8_t* address = nullptr;
    vector::UVector2 size;
    uint8_t bytes_per_pixel = 0;

    ProtectedFramebuffer(uint8_t* _address, vector::UVector2 _size, uint8_t _bytes_per_pixel) : address(_address), size(_size), bytes_per_pixel(_bytes_per_pixel) { };
public:
    inline uint8_t* getAddress() { return address; }
    inline vector::UVector2 getSize() { return size; }
    inline uint8_t getBytesPerPixel() { return bytes_per_pixel; }

    inline ProtectedFramebuffer() { };

    inline graphics::Framebuffer getWeakCopy() { return graphics::Framebuffer{ address, size, bytes_per_pixel }; }

    friend class Compositor;
};

class ContainerHandle
{
private:
    uint32_t handle_id = 0;
    Compositor* compositor = nullptr;

    bool has_been_resized = false;

    ContainerHandle(uint32_t id, Compositor* manager) : handle_id(id), compositor(manager) { };
public:
    inline bool isValid() { return handle_id != 0; }
    ProtectedFramebuffer* getFramebuffer();
    void blit();
    void repaint();
    void clear();
    void setDecoration(ContainerDecorationMode mode);
    void setTitle(String new_title);
    inline uint32_t getID() { return handle_id; }

    friend class Compositor;
    
    inline bool operator==(const ContainerHandle& b) const { return handle_id == b.handle_id; }
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

    uint32_t handle_id = 0;
    ProtectedFramebuffer framebuffer_details;
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
    ContainerHandle root_handle = ContainerHandle(0, nullptr);
    graphics::Framebuffer front_buffer;
    uint8_t* framebuffer_address = nullptr;
    Font* guiFont = nullptr;

    Array<ContainerLinkNode> handle_map;

    ProtectedFramebuffer overlay_framebuffer;
    bool overlay_visible = false;

    uint32_t getUnusedHandleID();

    void blit(Container* target, ProtectedFramebuffer source);
    void clear(Container* target);
    void repaint(Container* target);

    ProtectedFramebuffer* getContainerFramebuffer(ContainerHandle handle);
    void blitContainer(ContainerHandle handle);
    void repaintContainer(ContainerHandle handle);
    void clearContainer(ContainerHandle handle);
    void setContainerDecoration(ContainerHandle handle, ContainerDecorationMode mode);
    void setContainerTitle(ContainerHandle handle, String new_title);

    void initContainerTree();

    Pair<UVector2, UVector2> computeBounds(Container* target, bool account_for_border = false);
    Container* findContainer(ContainerHandle handle);

    enum Direction
    {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };

    Container* findClosestLeafContainer(Container* within, vector::UVector2 to, Direction preferred_direction);
public:
    Compositor(uint8_t* framebuffer_address, uint16_t width, uint16_t height, Font* font);

    void resizeCanvas(uint16_t width, uint16_t height);
    ContainerHandle divideContainer(ContainerHandle handle, ContainerSplitDecision split);
    ContainerHandle getRootContainer();
    void destroyContainer(ContainerHandle handle);

    ContainerHandle getNorthContainer(ContainerHandle handle);
    ContainerHandle getSouthContainer(ContainerHandle handle);
    ContainerHandle getEastContainer(ContainerHandle handle);
    ContainerHandle getWestContainer(ContainerHandle handle);

    ProtectedFramebuffer* getOverlayFramebuffer();
    void blitOverlayFramebuffer();
    void setOverlayVisible(bool visible);

    Compositor() = delete;
    Compositor(Compositor& other) = delete;
    Compositor(Compositor&& other) = delete;
    Compositor& operator=(const Compositor& other) = delete;
    Compositor& operator=(Compositor&& other) = delete;

    void debug();

    ~Compositor();

    friend class ContainerHandle;
};

}
}