#include <graphics.h>
#include <string.h>
#include <serial.h>
#include <binary_mesh.h>

namespace nov
{
namespace graphics
{
// TODO: comments in this file
using namespace vector;

void draw_box(const nov_uvector2& origin, const nov_uvector2& size, const nov_colour& col, const nov_framebuffer& framebuffer)
{
    uint32_t top_left = get_offset(origin, framebuffer.size);
    uint32_t top_right = (top_left + size.u) - 1;
    uint32_t bottom_left = top_left;
    uint32_t bottom_right = top_right;

    for (uint32_t t = 0; t < size.v; t++)
    {
        set_pixel(bottom_left, col, framebuffer);
        set_pixel(bottom_right, col, framebuffer);

        bottom_left += framebuffer.size.u;
        bottom_right += framebuffer.size.u;
    }

    top_right = top_left;
    bottom_left -= framebuffer.size.u;

    for (uint32_t t = 0; t < size.u; t++)
    {
        set_pixel(top_right, col, framebuffer);
        set_pixel(bottom_left, col, framebuffer);

        top_right++;
        bottom_left++;
    }
}

void fill_box(const nov_uvector2& origin, const nov_uvector2& size, const nov_colour& col, const nov_framebuffer& framebuffer)
{
    uint32_t offset = get_offset(origin, framebuffer.size);
    uint32_t end_offset = get_offset(origin+size-nov_uvector2{ 1,1 }, framebuffer.size);
    uint32_t x = 0;

    while (offset <= end_offset)
    {
        set_pixel(offset, col, framebuffer);
        offset++;
        x++;
        if (x >= size.u)
        {
            x = 0;
            offset -= size.u;
            offset += framebuffer.size.u;
        }
    }
}

void draw_line(const nov_uvector2& start, const nov_uvector2& end, const nov_colour& col, const nov_framebuffer& framebuffer)
{
    nov_uvector2 minimum = start;
    nov_uvector2 maximum = end;

    if (start.u > end.u)
    {
        minimum = end;
        maximum = start;
    }

    nov_uvector2 current = minimum;
    nov_uvector2 offset = nov_uvector2{0,0};

    float m = ((float)maximum.v-(float)minimum.v)/((float)maximum.u-(float)minimum.u);

    if (maximum.u == minimum.u)
    {
        if (start.v > end.v)
        {
            minimum = end;
            maximum = start;
        }
        else
        {
            minimum = start;
            maximum = end;
        }
        current = minimum;
        while (current.v < maximum.v)
        {
            set_pixel(get_offset(current, framebuffer.size), col, framebuffer);
            current.v++;
            if (current.v > framebuffer.size.v) break;
        }
    }
    else if (maximum.v == minimum.v) 
    {
        while (current.u < maximum.u)
        {
            set_pixel(get_offset(current, framebuffer.size), col, framebuffer);
            current.u++;
            if (current.u > framebuffer.size.u) break;
        }
    }
    else if (m >= 1.0f)
    {
        while (current.u < maximum.u)
        {
            set_pixel(get_offset(current, framebuffer.size), col, framebuffer);
            current.v++;
            offset.v++;
            current.u = (uint32_t)(((float)minimum.u)+((float)offset.v/m));
            if (current.u > framebuffer.size.u || current.v > framebuffer.size.v) break;
        }
    } else if (m >= 0)
    {
        while (current.u < maximum.u)
        {
            set_pixel(get_offset(current, framebuffer.size), col, framebuffer);
            current.u++;
            offset.u++;
            current.v = (uint32_t)(((float)minimum.v)+((float)offset.u*m));
            if (current.u > framebuffer.size.u || current.v > framebuffer.size.v) break;
        }
    } else if (m >= -1.0f)
    {
        while (current.u < maximum.u)
        {
            set_pixel(get_offset(current, framebuffer.size), col, framebuffer);
            current.u++;
            offset.u++;
            current.v = (uint32_t)(((float)minimum.v)+((float)offset.u*m));
            if (current.u > framebuffer.size.u || current.v > framebuffer.size.v) break;
        }
    }
    else if (m < -1.0f)
    {
        while (current.u < maximum.u)
        {
            set_pixel(get_offset(current, framebuffer.size), col, framebuffer);
            current.v--;
            offset.v++;
            current.u = (uint32_t)(((float)minimum.u)-((float)offset.v/m));
            if (current.u > framebuffer.size.u || current.v > framebuffer.size.v) break;
        }
    }
    return;
}

/**
 * recalculate stashed mesh data for rendering. this should be called whenever you've finished
 * modifying the mesh data and before using it for rendering
 * **/
void nov_mesh::update_mesh_data()
{
    // reallocate necessary buffers
    if (edge_vectors != 0x0) { delete[] edge_vectors; edge_vectors = 0x0; }
    if (edge_dots != 0x0) { delete[] edge_dots; edge_dots = 0x0; }
    if (inv_denoms != 0x0) { delete[] inv_denoms; inv_denoms = 0x0; }
    if (normals != 0x0) { delete[] normals; normals = 0x0; }

    uint16_t triangles_num = triangles_count/3;

    edge_vectors = new nov_fvector3[2*triangles_num];
    edge_dots = new float[3*triangles_num];
    inv_denoms = new float[triangles_num];
    normals = new nov_fvector3[triangles_num];

    // iterate over triangles and calculate constants
    nov_fvector3 v1,v2,v3,v1_2,v1_3,n;
    float d12_12,d12_13,d13_13;
    for (uint32_t i = 0; i < triangles_num; i++)
    {
        v1 = vertices[triangles[(i*3)]];
        v2 = vertices[triangles[(i*3)+1]];
        v3 = vertices[triangles[(i*3)+2]];

        // calculate v1->v2, v1->v3
        v1_2 = v2 - v1;
        edge_vectors[(i*2)] = v1_2;
        v1_3 = v3 - v1;
        edge_vectors[(i*2)+1] = v1_3;

        // calculate normal
        n = v1_2 % v1_3;
        normals[i] = n;

        // assign vertex normals, if they're blank (i.e. {0,0,0})
        if (vertex_normals[(i*3)] == nov_fvector3{0,0,0}) vertex_normals[(i*3)] = n;
        if (vertex_normals[(i*3)+1] == nov_fvector3{0,0,0}) vertex_normals[(i*3)+1] = n;
        if (vertex_normals[(i*3)+2] == nov_fvector3{0,0,0}) vertex_normals[(i*3)+2] = n;

        // calculate v1_2^v1_2, v1_2^v1_3, v1_3^v1_3
        d12_12 = mag_sq(v1_2);
        edge_dots[(i*3)] = d12_12;
        d12_13 = v1_2 ^ v1_3;
        edge_dots[(i*3)+1] = d12_13;
        d13_13 = mag_sq(v1_3);
        edge_dots[(i*3)+2] = d13_13;

        // calculate inverse denominator
        inv_denoms[i] = 1.0 / ((d12_12 * d13_13) - (d12_13 * d12_13));
    }

    // calculate object bounding box
    if (vertices_count == 0)
    {
        bounds.center = nov_fvector3{ 0,0,0 };
        bounds.radius = nov_fvector3{ 0,0,0 };
        bounds_mm_from_cr(bounds);
        return;
    }

    bounds.min = vertices[0];
    bounds.max = vertices[0];
    for (uint32_t i = 0; i < vertices_count; i++)
    {
        v1 = vertices[i];
        if (v1.x > bounds.max.x) bounds.max.x = v1.x;
        if (v1.y > bounds.max.y) bounds.max.y = v1.y;
        if (v1.z > bounds.max.z) bounds.max.z = v1.z;

        if (v1.y < bounds.min.y) bounds.min.y = v1.y;
        if (v1.z < bounds.min.z) bounds.min.z = v1.z;
        if (v1.x < bounds.min.x) bounds.min.x = v1.x;
    }

    bounds_cr_from_mm(bounds);
}

/**
 * read an .obj file string into the mesh data. overwrites all existing mesh data and 
 * recalculates mesh data when it's done
 * @param mesh_data pointer to the character array containing the object file data
 * @returns true if the mesh was successfully loaded from the data, or false if something went wrong
 * **/
bool nov_mesh::read_obj(const char* mesh_data)
{
    // lay a header over the start of the mesh data like a stencil
    file::nov_binary_mesh_header* header;
    header = (file::nov_binary_mesh_header*)mesh_data;
    
    // if the header is invalid, or there are no valid verts or tris, give up
    if (header->checksum != NOV_BINARY_MESH_HEADER_CHECKSUM 
     || header->vertex_buffer_length == 0 
     || header->triangle_buffer_length == 0) return false;

    // delete all the existing data
    if (vertices) delete[] vertices; vertices = 0x0;
    if (triangles) delete[] triangles; triangles = 0x0;
    if (materials) delete[] materials; materials = 0x0;
    if (material_indices) delete[] material_indices; material_indices = 0x0;
    if (uvs) delete[] uvs; uvs = 0x0;
    if (vertex_normals) delete[] vertex_normals; vertex_normals = 0x0;
    // and the other data
    if (normals) delete[] normals; normals = 0x0;
    if (edge_vectors) delete[] edge_vectors; edge_vectors = 0x0;
    if (edge_dots) delete[] edge_dots; edge_dots = 0x0;
    if (inv_denoms) delete[] inv_denoms; inv_denoms = 0x0;

    // reallocate all the buffers
    vertices_count = header->vertex_buffer_length;
    vertices = new nov_fvector3[vertices_count];
    com_1 << stream::mode::DEC;
    com_1 << "allocated space for " << vertices_count << " verts (" << (vertices_count * sizeof(nov_fvector3)) << ')' << stream::endl;
    vertices_capacity = vertices_count;

    triangles_count = header->triangle_buffer_length*3;
    triangles = new uint16_t[triangles_count];
    com_1 << "allocated space for " << triangles_count << " tris (" << (triangles_count * sizeof(uint16_t)) << ')' << stream::endl;
    triangles_capacity = triangles_count;

    material_indices = new uint16_t[header->triangle_buffer_length];
    com_1 << "allocated space for " << header->triangle_buffer_length << " mats (" << (header->triangle_buffer_length * sizeof(uint16_t)) << ')' << stream::endl;

    uvs = new nov_fvector2[triangles_count];
    com_1 << "allocated space for " << triangles_count << " uvs (" << (triangles_count * sizeof(nov_fvector2)) << ')' << stream::endl;
    vertex_normals = new nov_fvector3[triangles_count];
    com_1 << "allocated space for " << triangles_count << " norms (" << (triangles_count * sizeof(nov_fvector3)) << ')' << stream::endl;

    // TODO: materials?

    // iterate over the vertices and read them into the mesh vertex buffer
    file::nov_binary_mesh_vertex* verts = (file::nov_binary_mesh_vertex*)((char*)header + header->vertex_buffer_offset);
    for (uint32_t i = 0; i < header->vertex_buffer_length; i++)
    {
        vertices[i].x = verts[i].x;
        vertices[i].y = verts[i].y;
        vertices[i].z = verts[i].z;
    }

    // iterate over triangles and read them into the triangle buffer, and also the uvs at the same time
    file::nov_binary_mesh_triangle* tris = (file::nov_binary_mesh_triangle*)((char*)header + header->triangle_buffer_offset);
    file::nov_binary_mesh_normal* norms = (file::nov_binary_mesh_normal*)((char*)header + header->normal_buffer_offset);
    file::nov_binary_mesh_uv* coords = (file::nov_binary_mesh_uv*)((char*)header + header->uv_buffer_offset);
    uint32_t i3 = 0;
    for (uint32_t i = 0; i < header->triangle_buffer_length; i++)
    {
        triangles[i3] = tris[i].v0;
        vertex_normals[i3] = ((tris[i].flags & NOV_BINARY_MESH_TRIANGLE_N0) 
                            && (tris[i].n0 < header->normal_buffer_length))
                            ? nov_fvector3{ norms[tris[i].n0].x, norms[tris[i].n0].y, norms[tris[i].n0].z }
                            : nov_fvector3{ 0.0f, 0.0f, 0.0f };
        uvs[i3] = ((tris[i].flags & NOV_BINARY_MESH_TRIANGLE_U0) 
                 && (tris[i].u0 < header->uv_buffer_length))
                 ? nov_fvector2{ coords[tris[i].u0].u, coords[tris[i].u0].v }
                 : nov_fvector2{ 0.0f, 0.0f };
        i3++;
        triangles[i3] = tris[i].v1;
        vertex_normals[i3] = ((tris[i].flags & NOV_BINARY_MESH_TRIANGLE_N1) 
                            && (tris[i].n1 < header->normal_buffer_length))
                            ? nov_fvector3{ norms[tris[i].n1].x, norms[tris[i].n1].y, norms[tris[i].n1].z }
                            : nov_fvector3{ 0.0f, 0.0f, 0.0f };
        uvs[i3] = ((tris[i].flags & NOV_BINARY_MESH_TRIANGLE_U1) 
                 && (tris[i].u1 < header->uv_buffer_length))
                 ? nov_fvector2{ coords[tris[i].u1].u, coords[tris[i].u1].v }
                 : nov_fvector2{ 0.0f, 0.0f };
        i3++;
        triangles[i3] = tris[i].v2;
        vertex_normals[i3] = ((tris[i].flags & NOV_BINARY_MESH_TRIANGLE_N2) 
                            && (tris[i].n2 < header->normal_buffer_length))
                            ? nov_fvector3{ norms[tris[i].n2].x, norms[tris[i].n2].y, norms[tris[i].n2].z }
                            : nov_fvector3{ 0.0f, 0.0f, 0.0f };
        uvs[i3] = ((tris[i].flags & NOV_BINARY_MESH_TRIANGLE_U2) 
                 && (tris[i].u2 < header->uv_buffer_length))
                 ? nov_fvector2{ coords[tris[i].u2].u, coords[tris[i].u2].v }
                 : nov_fvector2{ 0.0f, 0.0f };
        i3++;
    }

    // pump the last bit of backing data, FIXME: disabled for now since most of the data isn't needed 
    update_mesh_data();

    // we're done
    return true;
}

uint16_t nov_mesh::count_vertices() { return vertices_count; }
uint16_t nov_mesh::count_triangles() { return triangles_count / 3; }

nov_mesh::nov_mesh() { }
nov_mesh::nov_mesh(const char* obj_data) { read_obj(obj_data); }

nov_mesh::~nov_mesh()
{
    if (vertices) delete[] vertices; vertices = 0x0;
    if (triangles) delete[] triangles; triangles = 0x0;
    if (materials) delete[] materials; materials = 0x0;
    if (material_indices) delete[] material_indices; material_indices = 0x0;
    if (uvs) delete[] uvs; uvs = 0x0;
    if (vertex_normals) delete[] vertex_normals; vertex_normals = 0x0;
    if (normals) delete[] normals; normals = 0x0;
    if (edge_vectors) delete[] edge_vectors; edge_vectors = 0x0;
    if (edge_dots) delete[] edge_dots; edge_dots = 0x0;
    if (inv_denoms) delete[] inv_denoms; inv_denoms = 0x0;
}

void bounds_mm_from_cr(nov_bounds& bounds)
{
    bounds.min = bounds.center - bounds.radius;
    bounds.max = bounds.center + bounds.radius;
}

void bounds_cr_from_mm(nov_bounds& bounds)
{
    bounds_fix_minmax(bounds);
    bounds.radius = (bounds.max - bounds.min) * 0.5f;
    bounds.center = bounds.min + bounds.radius;
}

void bounds_fix_minmax(nov_bounds& bounds)
{
    float tmp = 0.0f;
    if (bounds.min.x > bounds.max.x) { tmp = bounds.min.x; bounds.min.x = bounds.max.x; bounds.max.x = tmp; }
    if (bounds.min.y > bounds.max.y) { tmp = bounds.min.y; bounds.min.y = bounds.max.y; bounds.max.y = tmp; }
    if (bounds.min.z > bounds.max.z) { tmp = bounds.min.z; bounds.min.z = bounds.max.z; bounds.max.z = tmp; }
}

}
}