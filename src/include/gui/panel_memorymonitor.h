#include <gui/gui.h>
#include <graphics.h>

namespace nov
{
namespace gui
{

class nov_panel_memorymonitor : public nov_panel
{
private:
    static void _draw draw_function_stub;
public:

    inline nov_panel_memorymonitor()
    {
        draw_function_ptr = _draw;
        clear_on_draw = true;
        show_border = true;
    }
};

}
}