#include <gui/gui_base.h>
#include <graphics.h>

namespace nov
{
namespace gui
{

class PanelMeshrender : public Panel
{
private:
    static void _draw draw_function_stub;
public:
    Colour line_colour = nov_colour_gold;
    graphics::Mesh* mesh;
    FVector3 camera_look_direction;
    FVector3 camera_up_direction;
    FVector3 camera_position;

    inline PanelMeshrender()
    {
        draw_function_ptr = _draw;
    }
};

class PanelStar : public Panel
{
private:
    static void _draw draw_function_stub;
public:
    Colour foreground = nov_colour_white;
    Colour background = nov_colour_black;
    FVector2 uv = FVector2{ 1,1 };

    inline PanelStar()
    {
        show_border = false;
        clear_on_draw = false;
        draw_function_ptr = _draw;
    }
};

}
}