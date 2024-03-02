#include <gui/gui_base.h>
#include <graphics.h>

namespace nov
{
namespace gui
{

class nov_panel_textbox : public nov_panel
{
private:
    static void _draw draw_function_stub;
public:
    nov_colour text_colour;
    char* text;

    inline nov_panel_textbox()
    {
        draw_function_ptr = _draw;
        clear_on_draw = true;
        show_border = true;
    }
};

}
}