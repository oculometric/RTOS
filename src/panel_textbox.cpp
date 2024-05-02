#include <gui/panel_textbox.h>
#include <math.h>
#include <serial.h>

using namespace nov;

void gui::nov_panel_textbox::_draw draw_function_stub
{
    nov_panel_textbox* text_panel = static_cast<nov_panel_textbox*>(panel);
    if (!text_panel->font || !text_panel->font->bitmap) return;

    nov_uvector2 character_origin = frame.origin;
    uint32_t length = text_panel->text.get_length();

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

        // draw character
        uint8_t glyph_base_x = (chr % text_panel->font->tiles_per_row);
        uint8_t glyph_base_y = (chr / text_panel->font->tiles_per_row);
        uint8_t* glyph_start = text_panel->font->bitmap + ((glyph_base_x + (glyph_base_y * text_panel->font->char_height * text_panel->font->tiles_per_row)) * text_panel->font->char_width);

        for (uint8_t y = 0; y < text_panel->font->char_height; y++)
        {
            for (uint8_t x = 0; x < text_panel->font->char_width; x++)
            {
                if (glyph_start[x + (y * text_panel->font->bitmap_width)])
                {
                    nov_uvector2 position = character_origin + nov_uvector2{ x,y };
                    graphics::set_pixel(position.u + (position.v * framebuffer.size.u), text_panel->text_colour, framebuffer);
                }
            }
        }

        character_origin.u += text_panel->font->char_width + 1;
    }
}