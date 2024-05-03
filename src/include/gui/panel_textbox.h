#include <gui/gui_base.h>
#include <graphics.h>
#include <string.h>
#include <font.h>

namespace nov
{
namespace gui
{

class PanelTextbox : public Panel
{
private:
    static void _draw draw_function_stub;
public:
    Colour text_colour;
    String text;
    Font* font = 0;

    inline PanelTextbox()
    {
        draw_function_ptr = _draw;
        clear_on_draw = true;
        show_border = true;
    }
};

}
}