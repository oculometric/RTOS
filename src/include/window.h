#pragma once

#include <stdint.h>

#include <vector.h>
#include <array.h>
#include <colour.h>
#include <graphics.h>

#define WINDOW_SLICE_SIZE 5
#define WINDOW_SLICE_AREA WINDOW_SLICE_SIZE*WINDOW_SLICE_SIZE
#define WINDOW_SLICE_NUM 25

#define WINDOW_TOPLEFTCORNER 0
#define WINDOW_TOPEDGE 1
#define WINDOW_TOPJUNCTION 2
#define WINDOW_TOPRIGHTCORNER 3
#define WINDOW_LEFTJUNCTION 4
#define WINDOW_HORIZONTALEDGE 5
#define WINDOW_XJUNCTION 6
#define WINDOW_RIGHTJUNCTION 7
#define WINDOW_LEFTEDGE 8
#define WINDOW_SOLID 9
#define WINDOW_VERTICALEDGE 10
#define WINDOW_RIGHTEDGE 11
#define WINDOW_BOTTOMLEFTCORNER 12
#define WINDOW_BOTTOMEDGE 13
#define WINDOW_BOTTOMJUNCTION 14
#define WINDOW_BOTTOMRIGHTCORNER 15
#define WINDOW_SYMBOL_0 16
#define WINDOW_SYMBOL_1 17
#define WINDOW_SYMBOL_2 18
#define WINDOW_SYMBOL_3 19
#define WINDOW_SYMBOL_4 20
#define WINDOW_SYMBOL_5 21
#define WINDOW_SYMBOL_6 22
#define WINDOW_SYMBOL_7 23
#define WINDOW_SYMBOL_8 24

namespace nov
{
namespace gui
{

const uint8_t sliced_window[WINDOW_SLICE_SIZE*WINDOW_SLICE_SIZE*WINDOW_SLICE_NUM] =
{
    // top left corner
    0, 1, 1, 1, 1,
    3, 2, 2, 2, 2,
    3, 2, 2, 3, 3,
    3, 2, 1, 2, 4,
    3, 2, 1, 4, 2,
    // top edge
    1, 1, 1, 1, 1,
    2, 2, 2, 2, 2,
    3, 3, 3, 3, 3,
    4, 4, 4, 4, 4,
    1, 1, 1, 1, 1,
    // top junction
    1, 1, 1, 1, 1,
    2, 2, 2, 2, 2,
    3, 3, 3, 3, 3,
    4, 4, 4, 4, 4,
    1, 1, 4, 2, 1,
    // top right corner
    1, 1, 1, 1, 0,
    2, 2, 2, 1, 1,
    3, 3, 2, 2, 1,
    4, 3, 3, 2, 1,
    1, 4, 3, 2, 1,
    // left junction
    3, 2, 1, 4, 3,
    3, 2, 1, 4, 3,
    3, 2, 1, 4, 4,
    3, 2, 1, 4, 2,
    3, 2, 1, 4, 3,
    // horizontal edge
    2, 2, 2, 2, 2,
    3, 3, 3, 3, 3,
    4, 4, 4, 4, 4,
    1, 1, 1, 1, 1,
    2, 2, 2, 2, 2,
    // x junction
    2, 1, 4, 3, 2,
    3, 2, 4, 3, 3,
    4, 4, 4, 4, 4,
    1, 1, 4, 2, 1,
    2, 1, 4, 3, 2,
    // right junction
    1, 4, 3, 2, 1,
    2, 4, 3, 2, 1,
    4, 4, 3, 2, 1,
    1, 4, 3, 2, 1,
    1, 4, 3, 2, 1,
    // left edge
    3, 2, 1, 4, 3,
    3, 2, 1, 4, 3,
    3, 2, 1, 4, 3,
    3, 2, 1, 4, 3,
    3, 2, 1, 4, 3,
    // solid
    2, 2, 2, 2, 2,
    2, 2, 2, 2, 2,
    2, 2, 2, 2, 2,
    2, 2, 2, 2, 2,
    2, 2, 2, 2, 2,
    // vertical edge
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    // right edge
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    // bottom left corner
    3, 2, 1, 4, 3,
    3, 2, 1, 1, 4,
    3, 2, 2, 1, 1,
    4, 3, 2, 2, 2,
    0, 4, 3, 3, 3,
    // bottom edge
    3, 3, 3, 3, 3,
    4, 4, 4, 4, 4,
    1, 1, 1, 1, 1,
    2, 2, 2, 2, 2,
    3, 3, 3, 3, 3,
    // bottom junction
    3, 2, 4, 3, 3,
    4, 4, 4, 4, 4,
    1, 1, 1, 1, 1,
    2, 2, 2, 2, 2,
    3, 3, 3, 3, 3,
    // bottom right corner
    2, 4, 3, 2, 1,
    4, 2, 3, 2, 1,
    1, 1, 2, 2, 1,
    2, 2, 2, 2, 1,
    3, 3, 3, 3, 0,
    // r symbol 0
    2, 2, 2, 2, 2,
    1, 3, 3, 3, 1,
    1, 3, 2, 3, 2,
    1, 3, 2, 1, 2,
    1, 1, 2, 2, 2,
    // = symbol 1
    2, 2, 2, 2, 2,
    1, 3, 3, 3, 2,
    2, 2, 3, 2, 2,
    1, 3, 3, 3, 2,
    2, 1, 1, 1, 2,
    // T symbol 2
    2, 2, 2, 2, 2,
    2, 2, 2, 3, 2,
    1, 3, 3, 3, 2,
    1, 1, 1, 3, 2,
    2, 2, 2, 1, 2,
    // K symbol 3
    2, 2, 2, 2, 2,
    1, 3, 2, 3, 2,
    2, 1, 3, 3, 2,
    2, 2, 1, 3, 2,
    2, 2, 2, 1, 2,
    // D symbol 4
    2, 2, 2, 2, 2,
    1, 3, 3, 3, 2,
    2, 2, 2, 3, 2,
    1, 3, 3, 2, 2,
    1, 1, 1, 2, 2,
    // H symbol 5
    2, 2, 2, 2, 2,
    1, 3, 2, 3, 2,
    1, 3, 1, 3, 2,
    1, 3, 1, 3, 2,
    1, 1, 2, 1, 2,
    // X symbol 6
    2, 2, 2, 2, 2,
    1, 3, 2, 3, 2,
    2, 1, 3, 2, 2,
    1, 3, 1, 3, 2,
    2, 1, 2, 1, 2,
    // - symbol 7
    2, 2, 2, 2, 2,
    2, 2, 2, 2, 2,
    1, 3, 3, 3, 2,
    1, 1, 1, 1, 2,
    2, 2, 2, 2, 2,
    // O symbol 8
    2, 2, 2, 2, 2,
    1, 3, 3, 3, 2,
    1, 3, 2, 3, 2,
    1, 3, 3, 3, 2,
    1, 1, 1, 1, 2
};

using namespace nov::vector;
using namespace nov;

static const nov_colour colour_palette[5] =
{
    nov_colour{ 0,0,0 },
    nov_colour{ 238,211,0 },
    nov_colour{ 3,2,0 },
    nov_colour{ 184,103,0 },
    nov_colour{ 238,211,0 }
};

void draw_window(const nov_uvector2& origin, const nov_uvector2& size, uint8_t* buffer, const nov_uvector2& buffer_size);

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
 * passed to them
 * **/
class nov_panel
{
public:
    nov_colour fill_colour; // demo property

    void draw(nov_frame_data* frame, const graphics::nov_framebuffer& framebuffer);
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
 * **/ // TODO: remove unecessarry division
void calculate_frame_data(nov_frame_data* parent, const nov_fvector2& division, nov_frame_data* frame_a, nov_frame_data* frame_b);

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
    void draw_container(nov_container* container, nov_frame_data* frame);

public:
    nov_colour frame_outline_colour = nov_colour_gold;
    nov_colour frame_fill_colour = nov_colour_nearblack;
    uint8_t frame_menu_thickness = 6;

    /**
     * redraw the entire GUI tree
     * **/
    inline void draw_root() { draw_container(root_container, &root_container_frame); }
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