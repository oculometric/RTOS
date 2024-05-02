#include <gui/gui_base.h>
#include <graphics.h>
#include <string.h>
#include <font.h>

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
    nov_string text;
    nov_font* font = 0;

    inline nov_panel_textbox()
    {
        draw_function_ptr = _draw;
        clear_on_draw = true;
        show_border = true;
    }
};

}
}