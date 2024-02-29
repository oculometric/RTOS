#include <gui/panel_graphicsdemos.h>
#include <math.h>
#include <matrix.h>

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

    const vector::nov_fvector3 camera_right{ 1,0,0 };
    const vector::nov_fvector3 camera_up{ 0,0,1 };
    const vector::nov_fvector3 camera_backward{ 0,1,0 };

    const matrix::nov_matrix4_4<float> mat{ 4, 6, 7, 1, 
                                            3, 1, 2, 9,
                                            8, 5, 0, 2,
                                            2, 7, 3, 4 };
    
    auto inv = ~mat;
    auto trp = -mat;
    auto ident = mat * inv;
}


void gui::nov_panel_star::_draw draw_function_stub
{
    nov_panel_star* star_panel = static_cast<nov_panel_star*>(panel);

    // pointiness = 16
    const float c = 16;
    const float a = frame.size.u;
    const float b = frame.size.v;

    // cache constants
    const float scale_x_inv = (a * star_panel->uv.u) / (2 * (c - 1));
    const float scale_y = (2 * (c - 1)) / (b * star_panel->uv.v);
    const float half_x = a / 2.0f;
    const float half_y = b / 2.0f;

    const float y_limit_0 = half_y * (1 - star_panel->uv.v);
    const float y_limit_1 = half_y * (1 + star_panel->uv.v);

    // iterate over pixels
    float value = 0;
    uint32_t pixel_index = graphics::get_offset(frame.origin, framebuffer.size);
    uint32_t line_start = pixel_index;
    bool inside_star = false;
    uint32_t first_intersection = b;
    uint32_t second_intersection = b;
    for (uint32_t y = 0; y < b; y++)
    {
        inside_star = false;

        // calculate the first intersection with the y = y line
        if (y < y_limit_0 || y > y_limit_1)
        {
            // there are no intersections
            first_intersection = b;
            second_intersection = b;
        }
        else
        {
            // calculate the y component value and solve for the two intersections
            value = (abs(y - half_y) * scale_y + 1.0f);
            value = ((c / value) - 1.0f) * scale_x_inv;
            first_intersection = (uint32_t)(half_x - value);
            second_intersection = (uint32_t)(half_x + value) + 1;
        }

        for (uint32_t x = 0; x < frame.size.u; x++)
        {
            if (x == first_intersection || x == second_intersection) inside_star = !inside_star;
            graphics::set_pixel(pixel_index, inside_star ? star_panel->foreground : star_panel->background, framebuffer);
            pixel_index++;
        }
        line_start += framebuffer.size.u;
        pixel_index = line_start;
    }
}