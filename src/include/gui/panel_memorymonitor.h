#include <gui/gui_base.h>
#include <graphics.h>

namespace nov
{
namespace gui
{

class PanelMemoryMonitor : public Panel
{
private:
    static void _draw draw_function_stub;
public:

    inline PanelMemoryMonitor()
    {
        draw_function_ptr = _draw;
        clear_on_draw = true;
        show_border = true;
    }
};

}
}