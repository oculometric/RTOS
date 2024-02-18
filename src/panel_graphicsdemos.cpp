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
    nov_panel_star* star_panel = static_cast<nov_panel_star*>(panel);

    // pointiness = 16
    const float c = 16;
    const float a = frame.size.u;
    const float b = frame.size.v;

    // cache constants
    const float scale_x = (2 * (c - 1)) / (a * star_panel->uv.u);
    const float scale_y = (2 * (c - 1)) / (b * star_panel->uv.v);
    const float half_x = a / 2.0f;
    const float half_y = b / 2.0f;

    const float x_limit_0 = half_x * (1 - star_panel->uv.u);
    const float x_limit_1 = half_x * (1 + star_panel->uv.u);
    const float y_limit_0 = half_y * (1 - star_panel->uv.v);
    const float y_limit_1 = half_y * (1 + star_panel->uv.v);

    const float b_2 = (2 * b);
    const float xy_limit_0 = b_2 / a;

    // TODO: instead of sampling all pixels, rewrite this to calculate the start and stop points for 
    // graphic over a single line, then use that to fill much quicker

    // iterate over pixels
    float value = 0;
    float value_y = 0;
    float checking_value = 0;
    uint32_t pixel_index = graphics::get_offset(frame.origin, framebuffer.size);
    for (uint32_t y = 0; y < frame.size.v; y++)
    {
        if (y < y_limit_0 || y > y_limit_1)
        {
            // draw an empty line
            for (uint32_t x = 0; x < frame.size.u; x++)
            {
                graphics::set_pixel(pixel_index, star_panel->background, framebuffer);
                pixel_index++;
            }
        }
        else
        {
            value_y = (abs(y - half_y) * scale_y + 1.0f);
            for (uint32_t x = 0; x < frame.size.u; x++)
            {
                if (x < x_limit_0 || x > x_limit_1)
                    graphics::set_pixel(pixel_index, star_panel->background, framebuffer);
                else
                {
                    checking_value = xy_limit_0 * x;
                    if (checking_value < y) graphics::set_pixel(pixel_index, star_panel->background, framebuffer);
                    else if (checking_value > b + y) graphics::set_pixel(pixel_index, star_panel->background, framebuffer);
                    else if (checking_value < b - y) graphics::set_pixel(pixel_index, star_panel->background, framebuffer);
                    else if (checking_value > b_2 - y) graphics::set_pixel(pixel_index, star_panel->background, framebuffer);
                    else
                    {
                        value = (abs(x - half_x) * scale_x + 1.0f) * value_y;
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
                    }
                }
                pixel_index++;
            }
        }
        pixel_index -= frame.size.u;
        pixel_index += framebuffer.size.u;
    }
}