#pragma once

#include <stdint.h>

#include <vector.h>
#include <colour.h>
#include <graphics.h>

#define draw_function_stub (const nov_frame_data& frame, const graphics::nov_framebuffer& framebuffer, nov_panel* panel)

namespace nov
{
namespace gui
{

using namespace nov::vector;

/**
 * contains data about the frame (block of memory) into which something should be rendered
 * **/
struct nov_frame_data
{
    nov_uvector2 origin;            // offset of the start of the frame
    nov_uvector2 size;              // size of the frame
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
class nov_panel
{
protected:
    void (*draw_function_ptr)draw_function_stub;
    bool show_border = true;
    bool clear_on_draw = true;
public:
    inline void draw(const nov_frame_data& frame, const graphics::nov_framebuffer& framebuffer)
    {
        if (draw_function_ptr == 0x0) { com_1 << "invalid draw pointer" << stream::endl; return; }
        else draw_function_ptr(frame, framebuffer, this);
    }

    inline bool wants_border() { return show_border; }
    inline bool wants_clear() { return clear_on_draw; }
};

/**
 * represents a GUI block, which may either contain a panel, or be split into
 * two sub-containers. stores information about how these two sub-panels should be divided
 * **/
struct nov_container
{
    nov_container* child_a = 0x0;   // pointer to the left/top child container, if any
    nov_container* child_b = 0x0;   // pointer to the bottom/right child container, if any

    nov_panel* panel = 0x0;         // pointer to the panel to be rendered inside this container, if any

    nov_container* parent = 0x0;    // pointer to the parent of this container (only null for the root container)

    /**
     * division factor representing which fraction of this container should be given to the child containers.
     * only relevant if this container does not contain a panel. only one of either `division.u` or `division.v` should
     * be non-zero; whichever is non-zero will be used as the axis on which to divide the container (0 -> child_a fills
     * the container, 1 -> child_b fills the container)
     * **/
    nov_fvector2 division;
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
void calculate_frame_data(const nov_frame_data& parent, const nov_fvector2& division, nov_frame_data* frame_a, nov_frame_data* frame_b);

/**
 * splits a container into two halves with the given division. panel contents are moved into
 * child a
 * @param parent pointer to the parent which is being split in half
 * @param division divisor configuration representing what fraction of the parent frame should
 * be allocated to the a child frame versus the b child frame (0 = frame a fills parent, 1 = frame b fills parent)
 * **/
void split_container(nov_container* parent, const nov_fvector2& division);

/**
 * represents an overall object which handles drawing the GUI tree
 * **/
class nov_gui_manager
{
private:
    /**
     * pointer to the root container in the GUI tree, which fills the entire usable
     * framebuffer and contains all other frames
     * **/ 
    nov_container* root_container;
    // frame data about the root frame, used to calculate frame data for all child frames
    nov_frame_data root_container_frame;
    // information about the current framebuffer being drawn into
    graphics::nov_framebuffer framebuffer;

    /**
     * draw a container, calling draw on the panel inside it if it has one, then drawing its children
     * @param container pointer to the container to draw
     * @param frame pointer to the frame in which to draw the container
     * **/
    void draw_container(nov_container* container, const nov_frame_data& frame);

public:
    nov_colour frame_outline_colour = nov_colour_gold;
    nov_colour frame_fill_colour = nov_colour_nearblack;
    uint8_t frame_menu_thickness = 6;

    /**
     * redraw the entire GUI tree
     * **/
    inline void draw_root() { draw_container(root_container, root_container_frame); }
    /**
     * redraw a specific container and all children of it. useful if you only want to cause an update of
     * a specific area of the GUI without redrawing everything
     * @param container pointer to the container to be redrawn
     * 
     * @returns true if the container was redrawn, false if the container was not contained within the GUI tree
     * **/
    bool draw_specific(nov_container* container);

    /**
     * get a pointer to the root container of the GUI tree
     * @returns pointer to the root container
     * **/
    inline nov_container* get_root() { return root_container; }

    nov_gui_manager(const graphics::nov_framebuffer& framebuffer_info, nov_container* default_root = 0x0);
};
   
}
}