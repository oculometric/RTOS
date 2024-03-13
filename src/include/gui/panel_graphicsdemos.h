#include <gui/gui_base.h>
#include <graphics.h>

namespace nov
{
namespace gui
{

class nov_panel_meshrender : public nov_panel
{
private:
    static void _draw draw_function_stub;
public:
    nov_colour line_colour = nov_colour_gold;
    graphics::nov_mesh* mesh;
    nov_fvector3 camera_look_direction;
    nov_fvector3 camera_up_direction;
    nov_fvector3 camera_position;

    inline nov_panel_meshrender()
    {
        draw_function_ptr = _draw;
    }
};

class nov_panel_star : public nov_panel
{
private:
    static void _draw draw_function_stub;
public:
    nov_colour foreground = nov_colour_white;
    nov_colour background = nov_colour_black;
    nov_fvector2 uv = nov_fvector2{ 1,1 };

    inline nov_panel_star()
    {
        show_border = false;
        clear_on_draw = false;
        draw_function_ptr = _draw;
    }
};

}
}