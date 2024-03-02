#include <gui/panel_textbox.h>
#include <math.h>

using namespace nov;

void gui::nov_panel_textbox::_draw draw_function_stub
{
    nov_panel_textbox* text_panel = static_cast<nov_panel_textbox*>(panel);

    com_1 << text_panel->text << stream::endl;
}