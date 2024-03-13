#pragma once

#include <stdint.h>

#define NOV_BINARY_MESH_HEADER_CHECKSUM ('n' | ('b' << 8) | ('m' << 16) | ('h' << 24))

#define NOV_BINARY_MESH_TRIANGLE_N0 0b00000001
#define NOV_BINARY_MESH_TRIANGLE_N1 0b00000010
#define NOV_BINARY_MESH_TRIANGLE_N2 0b00000100
#define NOV_BINARY_MESH_TRIANGLE_U0 0b00001000
#define NOV_BINARY_MESH_TRIANGLE_U1 0b00010000
#define NOV_BINARY_MESH_TRIANGLE_U2 0b00100000

namespace nov
{
namespace file
{

struct nov_binary_mesh_header
{
    uint32_t checksum;                  // checksum to verify this is a binary mesh header

    uint32_t vertex_buffer_offset;      // offset in bytes from the start of this header of the list of vertices
    uint16_t vertex_buffer_length;      // number of vertices in the vertex buffer

    uint32_t triangle_buffer_offset;    // offset in bytes from the start of this header of the list of triangles
    uint16_t triangle_buffer_length;    // number of triangles in the triangle buffer

    uint32_t normal_buffer_offset;      // offset in bytes from the start of this header of the list of vertex normals
    uint16_t normal_buffer_length;      // number of normals in the vertex normal buffer

    uint32_t uv_buffer_offset;          // offset in bytes from the start of this header of the list of uvs
    uint16_t uv_buffer_length;          // number of normals in the uv buffer
};

// TODO: material buffer & specification

struct nov_binary_mesh_vertex
{
    float x,y,z;
};

typedef nov_binary_mesh_vertex nov_binary_mesh_normal;

struct nov_binary_mesh_uv
{
    float u,v;
};

struct nov_binary_mesh_triangle
{
    uint16_t v0,v1,v2;      // vertex indices of the corners of the faces, indices into the vertex array
    uint16_t n0,n1,n2;      // normal indices of the corners of the faces, indices into the vertex normal array
    uint16_t u0,u1,u2;      // uv indices of the corners of the faces, indices into the uv array
    uint8_t flags;          // packed byte with flags in bits:
                            // 0 - whether n0 is present
                            // 1 - whether n1 is present
                            // 2 - whether n2 is present
                            // 3 - whether u0 is present
                            // 4 - whether u1 is present
                            // 5 - whether u2 is present
    uint8_t material_index; // index into the material buffer of this triangle
};

}
}