#include <gui/panel_textbox.h>
#include <math.h>
#include <serial.h>

using namespace nov;

void gui::PanelTextbox::_draw draw_function_stub
{
    PanelTextbox* text_panel = static_cast<PanelTextbox*>(panel);
    if (!text_panel->font || !text_panel->font->bitmap) return;

    UVector2 character_origin = frame.origin;
    uint32_t length = text_panel->text.getLength();

    for (uint32_t char_ind = 0; char_ind < length; char_ind++)
    {
        char chr = text_panel->text[char_ind];
        // handle specials
        if (chr == '\n' || chr == '\r')
        {
            character_origin.u = frame.origin.u;
            if (chr == '\n')
            {
                character_origin.v += text_panel->font->char_height + 1;
            }
            continue;
        }
        if (chr == '\t')
        {
            character_origin.u += (text_panel->font->char_width + 1) * 4;
            continue;
        }

        graphics::drawCharacter(chr, character_origin, text_panel->text_colour, *(text_panel->font), framebuffer);
        
        character_origin.u += text_panel->font->char_width + 1;
    }
}