#include <gui/panel_graphicsdemos.h>
#include <math.h>
#include <matrix.h>
#include <serial.h>

using namespace nov;

void gui::PanelMeshrender::_draw draw_function_stub
{
    PanelMeshrender* meshrender_panel = (PanelMeshrender*)panel;
    
    if (!meshrender_panel->mesh) return;

    vector::FVector3 look = norm(meshrender_panel->camera_look_direction);
    vector::FVector3 up = norm(meshrender_panel->camera_up_direction);

    float tmp = look.x;
    look.x = look.y;
    look.y = tmp;

    tmp = up.x;
    up.x = up.y;
    up.y = tmp;

    // this represents the x-axis vector of the camera
    const vector::FVector3 camera_right = norm(look % up);
    // this represents the y-axis vector of the camera
    const vector::FVector3 camera_up = camera_right % look;
    // this represents the z-axis vector of the camera
    const vector::FVector3 camera_back = -look;

    // this represents the offset of the camera from the origin
    const vector::FVector3 camera_position = meshrender_panel->camera_position;

    // the world to camera transform is the inverse of the transform that takes the camera from its 'origin' state to its world position
    // so a matrix which represents the camera's transformation (position, rotation), the inverse needs to be applied to world-space stuff
    // to transform it into camera-space stuff

    // these two matrices represent the transformation of a point at the origin to where the camera is located/rotated in the world
    // therefore their inverses represent the transformation of a point located relative to the camera
    // i.e. multiplying by the inverse represents transforming the camera, and everything in the scene, such
    // that the camera is located at the origin and is facing along its normal axes (i.e. no rotation or offset)
    const matrix::FMatrix4 camera_position_mat{ 1.0f, 0.0f, 0.0f,  camera_position.y,
                                                    0.0f, 1.0f, 0.0f, -camera_position.x,
                                                    0.0f, 0.0f, 1.0f,  camera_position.z,
                                                    0.0f, 0.0f, 0.0f,  1.0f,              };
    
    const matrix::FMatrix4 camera_rotation_mat{  camera_right.x,  -camera_up.x,   camera_back.x,  0.0f,
                                                     -camera_right.y,  camera_up.y,  -camera_back.y,  0.0f,
                                                     camera_right.z,  -camera_up.z,   camera_back.z,  0.0f,
                                                     0.0f,             0.0f,          0.0f,           1.0f  };

    // apply the position first, then the rotation (flipped since inversing the matrices effectively flips the order of transformations)
    const matrix::FMatrix4 world_to_camera = ~(camera_position_mat * camera_rotation_mat);

    const float far_clip = 100.0f;
    const float near_clip = 0.001f;
    const float clip_rat = -far_clip / (far_clip - near_clip);
    const float fov_deg = 75.0f;
    const float s = 1.0f / tanf((fov_deg / 2.0f) * (MATH_PI / 180.0f));

    const matrix::FMatrix4 camera_to_view{ s,       0.0f,       0.0f,       0.0f,
                                               0.0f,    s,          0.0f,       0.0f,
                                               0.0f,    0.0f,       clip_rat,   clip_rat * near_clip,
                                               0.0f,    0.0f,       -1.0f,      0.0f                    };
    const matrix::FMatrix4 world_to_view = camera_to_view * world_to_camera;
    
    vector::FVector4 v_a_view;
    vector::FVector4 v_b_view;
    vector::FVector4 v_a_to_v_b;
    float gradient;
    vector::UVector2 v_a_window;
    vector::UVector2 v_b_window;

    // transform all vertices into view space (NDCs)
    FVector4* transformed_vertex_buffer = new FVector4[meshrender_panel->mesh->countVertices()];

    for (uint32_t i = 0; i < meshrender_panel->mesh->countVertices(); i++)
    {
        transformed_vertex_buffer[i] = world_to_view * FVector4{ meshrender_panel->mesh->vertices[i] };
        float tmp_z = transformed_vertex_buffer[i].w;
        transformed_vertex_buffer[i] /= tmp_z;
        transformed_vertex_buffer[i].z = tmp_z;
    }

    for (uint32_t i = 0; i < ((uint32_t)meshrender_panel->mesh->countTriangles() * 3) - 2; i++)
    {
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

        v_a_window = UVector2
        { 
            (uint32_t)(((v_a_view.x + 1.0f) * 0.5f) * frame.size.u), 
            (uint32_t)(((v_a_view.y + 1.0f) * 0.5f) * frame.size.v)
        }; v_a_window += frame.origin;

        v_b_window = UVector2 
        { 
            (uint32_t)(((v_b_view.x + 1.0f) * 0.5f) * frame.size.u), 
            (uint32_t)(((v_b_view.y + 1.0f) * 0.5f) * frame.size.v)
        }; v_b_window += frame.origin;

        graphics::drawLine(v_a_window, v_b_window, meshrender_panel->line_colour, framebuffer);
    }

    delete[] transformed_vertex_buffer;

    serial::com_1.flush();
}


void gui::PanelStar::_draw draw_function_stub
{
    PanelStar* star_panel = static_cast<PanelStar*>(panel);

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
    uint32_t pixel_index = graphics::getOffset(frame.origin, framebuffer.size);
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
            graphics::setPixel(pixel_index, inside_star ? star_panel->foreground : star_panel->background, framebuffer);
            pixel_index++;
        }
        line_start += framebuffer.size.u;
        pixel_index = line_start;
    }
}