#include <gui/panel_graphicsdemos.h>
#include <math.h>
#include <matrix.h>

using namespace nov;

void gui::nov_panel_cuberender::_draw draw_function_stub
{
    nov_panel_cuberender* cuberender_panel = (nov_panel_cuberender*)panel;

    // define vertex points
    const vector::nov_fvector4 t_0{ -cuberender_panel->radius.x, -cuberender_panel->radius.y, cuberender_panel->radius.z, 1.0f };
    const vector::nov_fvector4 t_1{ cuberender_panel->radius.x, -cuberender_panel->radius.y, cuberender_panel->radius.z, 1.0f };
    const vector::nov_fvector4 t_2{ cuberender_panel->radius.x, cuberender_panel->radius.y, cuberender_panel->radius.z, 1.0f };
    const vector::nov_fvector4 t_3{ -cuberender_panel->radius.x, cuberender_panel->radius.y, cuberender_panel->radius.z, 1.0f };

    const vector::nov_fvector4 b_0{ t_0.x, t_0.y, -t_0.z, 1.0f };
    const vector::nov_fvector4 b_1{ t_1.x, t_1.y, -t_1.z, 1.0f };
    const vector::nov_fvector4 b_2{ t_2.x, t_2.y, -t_2.z, 1.0f };
    const vector::nov_fvector4 b_3{ t_3.x, t_3.y, -t_3.z, 1.0f };

    const vector::nov_fvector3 camera_right{ 1,0,0 };
    const vector::nov_fvector3 camera_up{ 0,1,0 };
    const vector::nov_fvector3 camera_back{ 0,0,1 };

    const vector::nov_fvector3 camera_position{0.5f,1.2f,3.5f};

    const matrix::nov_fmatrix4 world_to_camera{ camera_right.x, camera_right.y, camera_right.z, -(camera_right ^ camera_position),
                                                camera_up.x,    camera_up.y,    camera_up.z,    (camera_up ^ camera_position),
                                                camera_back.x,  camera_back.y,  camera_back.z,  -(camera_back ^ camera_position),
                                                0.0f,           0.0f,           0.0f,           1.0f                                };

    const float far_clip = 100.0f;
    const float near_clip = 0.001f;
    const float clip_rat = -far_clip / (far_clip - near_clip);
    const float fov_deg = 85.0f;
    const float s = 1.0f / tanf((fov_deg / 2.0f) * (MATH_PI / 180.0f));

    const matrix::nov_fmatrix4 camera_to_view{ s,       0.0f,       0.0f,       0.0f,
                                               0.0f,    s,          0.0f,       0.0f,
                                               0.0f,    0.0f,       clip_rat,   clip_rat * near_clip,
                                               0.0f,    0.0f,       -1.0f,      0.0f                    };
    const matrix::nov_fmatrix4 world_to_view = camera_to_view * world_to_camera;
    
    const vector::nov_fvector4* path[16] = 
    {
        &t_0, &t_1, &t_2, &t_3, &t_0, &b_0, &b_1, &b_2, &b_3, &b_0, &b_1, &t_1, &t_2, &b_2, &b_3, &t_3
    };
    
    com_1 << world_to_camera << stream::endl;
    com_1 << camera_to_view << stream::endl;
    com_1 << world_to_view << stream::endl;
    
    vector::nov_fvector4 v_a_view;
    vector::nov_fvector4 v_b_view;
    vector::nov_uvector2 v_a_window;
    vector::nov_uvector2 v_b_window;

    for (uint32_t i = 0; i < sizeof(path)/sizeof(vector::nov_fvector4*) - 1; i++)
    {
        v_a_view = world_to_view * *(path[i]);
        v_a_view /= v_a_view.w;
        v_b_view = world_to_view * *(path[i+1]);
        v_b_view /= v_b_view.w;

        v_a_window = nov_uvector2
        { 
            (uint32_t)(((v_a_view.x + 1.0f) * 0.5f) * frame.size.u), 
            (uint32_t)(((v_a_view.y + 1.0f) * 0.5f) * frame.size.v)
        }; v_a_window += frame.origin;

        v_b_window = nov_uvector2 
        { 
            (uint32_t)(((v_b_view.x + 1.0f) * 0.5f) * frame.size.u), 
            (uint32_t)(((v_b_view.y + 1.0f) * 0.5f) * frame.size.v)
        }; v_b_window += frame.origin;

        graphics::draw_line(v_a_window, v_b_window, cuberender_panel->line_colour, framebuffer);

        com_1 << *(path[i]) << " -> " << v_a_view << stream::endl;
        com_1 << *(path[i+1]) << " -> " << v_b_view << stream::endl;
    }

    com_1.flush();

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