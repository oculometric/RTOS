#pragma once

#include <stdint.h>

#include <vector.h>
#include <colour.h>
#include <graphics.h>
#include <array.h>
#include <string.h>

#define draw_function_stub (const FrameData& frame, const graphics::Framebuffer& framebuffer, Panel* panel)

namespace nov
{
namespace gui
{

using namespace nov::vector;

/**
 * contains data about the frame (block of memory) into which something should be rendered
 * **/
struct FrameData
{
    UVector2 origin;            // offset of the start of the frame
    UVector2 size;              // size of the frame
};

/**
 * represents an area in the GUI which can be drawn into. must be contained within a container,
 * and draw is called by a nov_gui_manager. this class can be extended to customise behaviour and
 * display different things. this is where the GUI actually lives. draw calls MUST respect the bounds
 * passed to them.
 * 
 * extending this class is done by publicly subclassing it, creating your own `private static void` 
 * function to perform drawing,and writing a constructor which assigns that function as a pointer
 * to the `draw_function_ptr` variable. do NOT override the `draw` function directly.
 * 
 * subclasses may also use this opportunity to set the `show_border` and `clear_on_draw` properties
 * **/
class Panel
{
protected:
    void (*draw_function_ptr)draw_function_stub;
    bool show_border = true;
    bool clear_on_draw = true;
public:
    String name;

    inline void draw(const FrameData& frame, const graphics::Framebuffer& framebuffer)
    {
        if (draw_function_ptr == 0x0) { return; }
        else draw_function_ptr(frame, framebuffer, this);
    }

    inline bool wantsBorder() { return show_border; }
    inline bool wantsClear() { return clear_on_draw; }
};

/**
 * represents a GUI block, which may either contain a panel, or be split into
 * two sub-containers. stores information about how these two sub-panels should be divided
 * **/
struct Container
{
    Container* child_a = 0x0;   // pointer to the left/top child container, if any
    Container* child_b = 0x0;   // pointer to the bottom/right child container, if any

    Panel* panel = 0x0;         // pointer to the panel to be rendered inside this container, if any

    Container* parent = 0x0;    // pointer to the parent of this container (only null for the root container)

    /**
     * division factor representing which fraction of this container should be given to the child containers.
     * only relevant if this container does not contain a panel. only one of either `division.u` or `division.v` should
     * be non-zero; whichever is non-zero will be used as the axis on which to divide the container (0 -> child_a fills
     * the container, 1 -> child_b fills the container)
     * **/
    FVector2 division;
};

/**
 * contains information about a cached frame and the container pointer it is relevant to
 * **/
struct FrameCache
{
    FrameData frame_data;      // cached (pre-calculated) frame data
    Container* container;       // pointer to the container for which this is relevant
};

/**
 * computes the frame data for two child frames based on a parent frame. does the maths to
 * split the parent in half using the division factor
 * @param parent pointer to parent frame data struct
 * @param division division factor representing what fraction of the parent frame should be allocated
 * to the a frame versus the b frame (0 = frame a fills parent, 1 = frame b fills parent)
 * @param frame_a pointer to the frame data struct where the left/top frame will be described
 * @param frame_b pointer to the frame data struct where the bottom/right frame will be described
 * **/
void calculateFrameData(const FrameData& parent, const FVector2& division, FrameData* frame_a, FrameData* frame_b);

/**
 * splits a container into two halves with the given division. panel contents are moved into
 * child a
 * @param parent pointer to the parent which is being split in half
 * @param division divisor configuration representing what fraction of the parent frame should
 * be allocated to the a child frame versus the b child frame (0 = frame a fills parent, 1 = frame b fills parent)
 * **/
void splitContainer(Container* parent, const FVector2& division);

/**
 * represents an overall object which handles drawing the GUI tree
 * **/
class GuiManager
{
private:
    /**
     * pointer to the root container in the GUI tree, which fills the entire usable
     * framebuffer and contains all other frames
     * **/ 
    Container* root_container;
    // frame data about the root frame, used to calculate frame data for all child frames
    FrameData root_container_frame;
    // information about the current framebuffer being drawn into
    graphics::Framebuffer framebuffer;
    // array of cached information about the container tree
    Array<FrameCache> frame_cache;

    /**
     * draw a container, calling draw on the panel inside it if it has one, then drawing its children
     * @param container pointer to the container to draw
     * @param frame pointer to the frame in which to draw the container
     * **/
    void drawContainer(Container* container, const FrameData& frame);

public:
    Colour frame_outline_colour = nov_colour_gold;
    Colour frame_fill_colour = nov_colour_nearblack;
    uint8_t frame_menu_thickness = 6;
    Font* guiFont = 0;

    /**
     * redraw the entire GUI tree
     * **/
    inline void drawRoot() { drawContainer(root_container, root_container_frame); }
    /**
     * redraw a specific container and all children of it. useful if you only want to cause an update of
     * a specific area of the GUI without redrawing everything
     * @param container pointer to the container to be redrawn
     * 
     * @returns true if the container was redrawn, false if the container was not contained within the GUI tree
     * **/
    bool drawSpecific(Container* container);

    /**
     * get a pointer to the root container of the GUI tree
     * @returns pointer to the root container
     * **/
    inline Container* getRoot() { return root_container; }

    GuiManager(const graphics::Framebuffer& framebuffer_info, Container* default_root = 0x0);
};
   
}
}