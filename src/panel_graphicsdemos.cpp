#include <gui/panel_graphicsdemos.h>
#include <math.h>

using namespace nov;

void gui::nov_panel_cuberender::_draw draw_function_stub
{
    nov_panel_cuberender* cuberender_panel = (nov_panel_cuberender*)panel;

    // define vertex points
    const vector::nov_fvector3 t_0{ -cuberender_panel->radius.x, -cuberender_panel->radius.y, cuberender_panel->radius.z };
    const vector::nov_fvector3 t_1{ cuberender_panel->radius.x, -cuberender_panel->radius.y, cuberender_panel->radius.z };
    const vector::nov_fvector3 t_2{ cuberender_panel->radius.x, cuberender_panel->radius.y, cuberender_panel->radius.z };
    const vector::nov_fvector3 t_3{ -cuberender_panel->radius.x, cuberender_panel->radius.y, cuberender_panel->radius.z };

    const vector::nov_fvector3 b_0{ t_0.x, t_0.y, -t_0.z };
    const vector::nov_fvector3 b_1{ t_1.x, t_1.y, -t_1.z };
    const vector::nov_fvector3 b_2{ t_2.x, t_2.y, -t_2.z };
    const vector::nov_fvector3 b_3{ t_3.x, t_3.y, -t_3.z };
}


void gui::nov_panel_star::_draw draw_function_stub
{
    nov_panel_star* star_panel = (nov_panel_star*)panel;

    // pointiness = 16
    const float c = 16;
    // star may be stretched on uv, but will fill frame relative to that stretch
    const float u = star_panel->uv.u;
    const float v = star_panel->uv.v;

    // cache constants
    const float scale_x = (2 * (c - 1)) / (frame.size.u * u);
    const float scale_y = (2 * (c - 1)) / (frame.size.v * v);
    const float half_x = frame.size.u / 2.0f;
    const float half_y = frame.size.v / 2.0f;

    // iterate over pixels
    float value = 0;
    uint32_t pixel_index = graphics::get_offset(frame.origin, framebuffer.size);
    for (uint32_t y = 0; y < frame.size.v; y++)
    {
        for (uint32_t x = 0; x < frame.size.u; x++)
        {
            value = ((float)abs(x - half_x) * scale_x + 1.0f) * ((float)abs(y - half_y) * scale_y + 1.0f);
            if (value < c)
            {
                // draw pixel as filled
                graphics::set_pixel(pixel_index, star_panel->foreground, framebuffer);
            }
            else
            {
                // draw pixel empty
                graphics::set_pixel(pixel_index, star_panel->background, framebuffer);
            }
            pixel_index++;
        }
        pixel_index -= frame.size.u;
        pixel_index += framebuffer.size.u;
    }
}