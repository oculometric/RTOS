#include <gui/gui_base.h>
#include <graphics.h>

namespace nov
{
namespace gui
{


class nov_panel_cuberender : public nov_panel
{
private:
    static void _draw draw_function_stub;
public:
    nov_colour line_colour;
    nov_fvector3 radius;
    nov_fvector3 rotation;

    inline nov_panel_cuberender()
    {
        draw_function_ptr = _draw;
        line_colour = nov_colour_gold;
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