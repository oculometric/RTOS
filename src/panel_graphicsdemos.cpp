#include <gui/panel_graphicsdemos.h>
#include <math.h>
#include <matrix.h>
#include <serial.h>

using namespace nov;

void gui::nov_panel_meshrender::_draw draw_function_stub
{
    nov_panel_meshrender* meshrender_panel = (nov_panel_meshrender*)panel;
    
    if (!meshrender_panel->mesh) return;

    // TODO: 3D rotation
    const vector::nov_fvector3 camera_right = meshrender_panel->camera_look_direction % -meshrender_panel->camera_up_direction;
    const vector::nov_fvector3 camera_up = meshrender_panel->camera_look_direction % camera_right;
    const vector::nov_fvector3 camera_back = -meshrender_panel->camera_look_direction;

    const vector::nov_fvector3 camera_position = meshrender_panel->camera_position;

    const matrix::nov_fmatrix4 world_to_camera{ camera_right.x, camera_right.y, camera_right.z, -(camera_right ^ camera_position),
                                                -camera_up.x,   -camera_up.y,   -camera_up.z,    (camera_up ^ camera_position),
                                                camera_back.x,  camera_back.y,  camera_back.z,  -(camera_back ^ camera_position),
                                                0.0f,           0.0f,           0.0f,           1.0f                                };

    // const vector::nov_fvector3 euler = meshrender_panel->camera_rotation;
    // const matrix::nov_fmatrix4 rotate_camera_x{ 1.0f, 0.0f,          0.0f,           0.0f,
    //                                             0.0f, cosf(euler.x), -sinf(euler.x), 0.0f,
    //                                             0.0f, sinf(euler.x), cosf(euler.x),  0.0f,
    //                                             0.0f, 0.0f,          0.0f,           1.0f  };
    
    // const matrix::nov_fmatrix4 rotate_camera_y{ cosf(euler.y),  0.0f, sinf(euler.y),  0.0f,
    //                                             0.0f,           1.0f, 0.0f,           0.0f,
    //                                             -sinf(euler.y), 0.0f, cosf(euler.x),  0.0f,
    //                                             0.0f,           0.0f, 0.0f,           1.0f  };

    // const matrix::nov_fmatrix4 rotate_camera_z{ cosf(euler.z),  -sinf(euler.z), 0.0f,  0.0f,
    //                                             sinf(euler.z),  cosf(euler.z),  0.0f,  0.0f,
    //                                             0.0f,           0.0f,           1.0f,  0.0f,
    //                                             0.0f,           0.0f,           0.0f,  1.0f  };

    // const matrix::nov_fmatrix4 rotate_camera = rotate_camera_x * rotate_camera_y * rotate_camera_z;

    const float far_clip = 100.0f;
    const float near_clip = 0.001f;
    const float clip_rat = -far_clip / (far_clip - near_clip);
    const float fov_deg = 85.0f;
    const float s = 1.0f / tanf((fov_deg / 2.0f) * (MATH_PI / 180.0f));

    const matrix::nov_fmatrix4 camera_to_view{ s,       0.0f,       0.0f,       0.0f,
                                               0.0f,    s,          0.0f,       0.0f,
                                               0.0f,    0.0f,       clip_rat,   clip_rat * near_clip,
                                               0.0f,    0.0f,       -1.0f,      0.0f                    };
    const matrix::nov_fmatrix4 world_to_view = camera_to_view /* * ~rotate_camera*/ * world_to_camera;
    
    vector::nov_fvector4 v_a_view;
    vector::nov_fvector4 v_b_view;
    vector::nov_fvector4 v_a_to_v_b;
    float gradient;
    vector::nov_uvector2 v_a_window;
    vector::nov_uvector2 v_b_window;

    // transform all vertices into view space (NDCs)
    nov_fvector4* transformed_vertex_buffer = new nov_fvector4[meshrender_panel->mesh->count_vertices()];

    for (uint32_t i = 0; i < meshrender_panel->mesh->count_vertices(); i++)
    {
        transformed_vertex_buffer[i] = world_to_view * nov_fvector4{ meshrender_panel->mesh->vertices[i] };
        transformed_vertex_buffer[i] /= transformed_vertex_buffer[i].w;
    }

    for (uint32_t i = 0; i < (meshrender_panel->mesh->count_triangles() * 3) - 2; i++)
    {
        if ((meshrender_panel->mesh->normals[i/3] ^ camera_back) < 0.0f) { i += 2; continue; }
        if (i % 3 <= 1)
        {
            v_a_view = transformed_vertex_buffer[meshrender_panel->mesh->triangles[i]];
            v_b_view = transformed_vertex_buffer[meshrender_panel->mesh->triangles[i+1]];
        }
        else
        {
            v_a_view = transformed_vertex_buffer[meshrender_panel->mesh->triangles[i]];
            v_b_view = transformed_vertex_buffer[meshrender_panel->mesh->triangles[i-2]];
        }

        if (v_a_view.z < 0.0f || v_b_view.z < 0.0f) continue;

        v_a_to_v_b = v_b_view - v_a_view;
        gradient = v_a_to_v_b.y / v_a_to_v_b.x;

        // if the line is completely off the screen, ignore it
        if ((v_a_view.x > 1.0f && v_b_view.x > 1.0f)
         || (v_a_view.x < -1.0f && v_b_view.x < -1.0f)
         || (v_a_view.y > 1.0f && v_b_view.y > 1.0f)
         || (v_a_view.y < -1.0f && v_b_view.y < -1.0f)) continue;

        // clip lines to the screen box
        if (abs(v_a_view.x) >= 1.0f)
        {
            float scalar = (abs(v_a_view.x) - 1.0f) * sign(v_a_view.x);
            v_a_view.x -= scalar;
            v_a_view.y -= scalar * gradient;
        }
        if (abs(v_a_view.y) >= 1.0f)
        {
            float scalar = (abs(v_a_view.y) - 1.0f) * sign(v_a_view.y);
            v_a_view.y -= scalar;
            v_a_view.x -= scalar / gradient;
        }

        if (abs(v_b_view.x) >= 1.0f)
        {
            float scalar = (abs(v_b_view.x) - 1.0f) * sign(v_b_view.x);
            v_b_view.x -= scalar;
            v_b_view.y -= scalar * gradient;
        }
        if (abs(v_b_view.y) >= 1.0f)
        {
            float scalar = (abs(v_b_view.y) - 1.0f) * sign(v_b_view.y);
            v_b_view.y -= scalar;
            v_b_view.x -= scalar / gradient;
        }

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

        graphics::draw_line(v_a_window, v_b_window, meshrender_panel->line_colour, framebuffer);
    }

    delete[] transformed_vertex_buffer;

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