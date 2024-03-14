#pragma once

#include <stdint.h>

#include <vector.h>
#include <graphics/bounds.h>
#include <graphics/material.h>

namespace nov
{
namespace graphics
{

// stores a block of triangles for the renderer to render
// for optimal performance, triangles should be distributed to objects
// according to their localisation in the scene
class nov_mesh
{
public:
    // lists all the vertices used by the object
    vector::nov_fvector3* vertices = 0x0;
    // stores indices of vertices used by each triangle, length should be 3 times the number of triangles
    uint16_t* triangles = 0x0;
    // list of pointers to materials used on this object
    nov_material** materials = 0x0;
    // indices of materials used by triangles, length should be equal to number of triangles
    uint16_t* material_indices = 0x0;
    // stores UV coordinates for vertices, length should be 3 times the number of triangles
    vector::nov_fvector2* uvs = 0x0;
    // stores normals for vertices, used for surface smoothing. length should be 3 times number of triangles
    vector::nov_fvector3* vertex_normals = 0x0;

    // stores normal data for each triangle, length should be equal to number of triangles. never directly touch this
    vector::nov_fvector3* normals = 0x0;
    // stores the 1->2 and 1->3 vectors for each triangle, length should be 2 times number of triangles. never directly touch this
    vector::nov_fvector3* edge_vectors = 0x0;
    // stores constants for raycasting, length should be 3 times number of triangles. never directly touch this
    float* edge_dots = 0x0;
    // inverse denominators, length should be equal to number of triangles. never directly touch this
    float* inv_denoms = 0x0;
    // bounding box of the object. never directly touch this
    nov_bounds bounds;

private:
    // list length data for vertices and triangles. never directly touch these
    uint16_t vertices_count = 0;
    uint16_t vertices_capacity = 0;
    uint16_t triangles_count = 0;
    uint16_t triangles_capacity = 0;

    void deallocate_buffers();

public:
    void update_mesh_data();
    bool read_obj(const char* mesh_data);

    uint16_t count_vertices();
    uint16_t count_triangles();

    nov_mesh();
    nov_mesh(const char* obj_data);

    ~nov_mesh();
};

}
}