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

void copyBox(const UVector2& tar_offset, const UVector2& src_offset, const UVector2& size, const Framebuffer& target, const Framebuffer& source)
{
    // TODO: make this more efficient by copying as dwords (handle edges nicely)
    uint32_t t_pos = tar_offset.u + (tar_offset.v * target.size.u);
    uint32_t s_pos = src_offset.u + (src_offset.v * source.size.u);
    uint32_t t_wrap = target.size.u - size.u;
    uint32_t s_wrap = source.size.u - size.u;
    for (uint32_t y = 0; y < size.v; y++)
    {
        for (uint32_t x = 0; x < size.u; x++)
        {
            target.address[(t_pos * 3) + 0] = source.address[(s_pos * 3) + 0];
            target.address[(t_pos * 3) + 1] = source.address[(s_pos * 3) + 1];
            target.address[(t_pos * 3) + 2] = source.address[(s_pos * 3) + 2];
            t_pos++;
            s_pos++;
        }
        t_pos += t_wrap;
        s_pos += s_wrap;
    }
}

void drawBox(const UVector2& origin, const UVector2& size, const Colour& col, const Framebuffer& framebuffer)
{
    uint32_t top_left = getOffset(origin, framebuffer.size);
    uint32_t top_right = (top_left + size.u) - 1;
    uint32_t bottom_left = top_left;
    uint32_t bottom_right = top_right;

    for (uint32_t t = 0; t < size.v; t++)
    {
        setPixel(bottom_left, col, framebuffer);
        setPixel(bottom_right, col, framebuffer);

        bottom_left += framebuffer.size.u;
        bottom_right += framebuffer.size.u;
    }

    top_right = top_left;
    bottom_left -= framebuffer.size.u;

    for (uint32_t t = 0; t < size.u; t++)
    {
        setPixel(top_right, col, framebuffer);
        setPixel(bottom_left, col, framebuffer);

        top_right++;
        bottom_left++;
    }
}

void fillBox(const UVector2& origin, const UVector2& size, const Colour& col, const Framebuffer& framebuffer)
{
    uint32_t offset = getOffset(origin, framebuffer.size);
    uint32_t end_offset = getOffset(origin + size - UVector2{ 1,1 }, framebuffer.size);
    uint32_t x = 0;

    while (offset <= end_offset)
    {
        setPixel(offset, col, framebuffer);
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

void drawLine(const UVector2& start, const UVector2& end, const Colour& col, const Framebuffer& framebuffer)
{
    UVector2 minimum = start;
    UVector2 maximum = end;

    if (start.u > end.u)
    {
        minimum = end;
        maximum = start;
    }

    UVector2 current = minimum;
    UVector2 offset = UVector2{ 0,0 };

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
            setPixel(getOffset(current, framebuffer.size), col, framebuffer);
            current.v++;
            if (current.v > framebuffer.size.v) break;
        }
    }
    else if (maximum.v == minimum.v) 
    {
        while (current.u < maximum.u)
        {
            setPixel(getOffset(current, framebuffer.size), col, framebuffer);
            current.u++;
            if (current.u > framebuffer.size.u) break;
        }
    }
    else if (m >= 1.0f)
    {
        while (current.u < maximum.u)
        {
            setPixel(getOffset(current, framebuffer.size), col, framebuffer);
            current.v++;
            offset.v++;
            current.u = (uint32_t)(((float)minimum.u)+((float)offset.v/m));
            if (current.u > framebuffer.size.u || current.v > framebuffer.size.v) break;
        }
    } else if (m >= 0)
    {
        while (current.u < maximum.u)
        {
            setPixel(getOffset(current, framebuffer.size), col, framebuffer);
            current.u++;
            offset.u++;
            current.v = (uint32_t)(((float)minimum.v)+((float)offset.u*m));
            if (current.u > framebuffer.size.u || current.v > framebuffer.size.v) break;
        }
    } else if (m >= -1.0f)
    {
        while (current.u < maximum.u)
        {
            setPixel(getOffset(current, framebuffer.size), col, framebuffer);
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
            setPixel(getOffset(current, framebuffer.size), col, framebuffer);
            current.v--;
            offset.v++;
            current.u = (uint32_t)(((float)minimum.u)-((float)offset.v/m));
            if (current.u > framebuffer.size.u || current.v > framebuffer.size.v) break;
        }
    }
    return;
}

/**
 * delete all the buffers (if they actually exist). oh boy you better not do any silly 
 * stuff after we delete the buffers!
 * **/
void Mesh::deallocateBuffers()
{
    if (vertices) delete[] vertices;
    vertices = 0x0;
    if (triangles) delete[] triangles;
    triangles = 0x0;
    if (materials) delete[] materials;
    materials = 0x0;
    if (material_indices) delete[] material_indices;
    material_indices = 0x0;
    if (uvs) delete[] uvs;
    uvs = 0x0;
    if (vertex_normals) delete[] vertex_normals;
    vertex_normals = 0x0;
    // and the other data
    if (normals) delete[] normals;
    normals = 0x0;
    if (edge_vectors) delete[] edge_vectors;
    edge_vectors = 0x0;
    if (edge_dots) delete[] edge_dots;
    edge_dots = 0x0;
    if (inv_denoms) delete[] inv_denoms;
    inv_denoms = 0x0;

    memory::mConsolidate();
}

/**
 * recalculate stashed mesh data for rendering. this should be called whenever you've finished
 * modifying the mesh data and before using it for rendering
 * **/
void Mesh::updateMeshData()
{
    // reallocate necessary buffers
    if (edge_vectors != 0x0) { delete[] edge_vectors; edge_vectors = 0x0; }
    if (edge_dots != 0x0) { delete[] edge_dots; edge_dots = 0x0; }
    if (inv_denoms != 0x0) { delete[] inv_denoms; inv_denoms = 0x0; }
    if (normals != 0x0) { delete[] normals; normals = 0x0; }

    uint16_t triangles_num = triangles_count/3;

    edge_vectors = new FVector3[2*triangles_num];
    edge_dots = new float[3*triangles_num];
    inv_denoms = new float[triangles_num];
    normals = new FVector3[triangles_num];

    // iterate over triangles and calculate constants
    FVector3 v1,v2,v3,v1_2,v1_3,n;
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
        if (vertex_normals[(i*3)] == FVector3{0,0,0}) vertex_normals[(i*3)] = n;
        if (vertex_normals[(i*3)+1] == FVector3{0,0,0}) vertex_normals[(i*3)+1] = n;
        if (vertex_normals[(i*3)+2] == FVector3{0,0,0}) vertex_normals[(i*3)+2] = n;

        // calculate v1_2^v1_2, v1_2^v1_3, v1_3^v1_3
        d12_12 = magSq(v1_2);
        edge_dots[(i*3)] = d12_12;
        d12_13 = v1_2 ^ v1_3;
        edge_dots[(i*3)+1] = d12_13;
        d13_13 = magSq(v1_3);
        edge_dots[(i*3)+2] = d13_13;

        // calculate inverse denominator
        inv_denoms[i] = 1.0 / ((d12_12 * d13_13) - (d12_13 * d12_13));
    }

    // calculate object bounding box
    if (vertices_count == 0)
    {
        bounds.center = FVector3{ 0,0,0 };
        bounds.radius = FVector3{ 0,0,0 };
        boundsMinMaxFromCenterRadius(bounds);
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

    boundsCenterRadiusFromMinMax(bounds);
}

/**
 * read an .obj file string into the mesh data. overwrites all existing mesh data and 
 * recalculates mesh data when it's done
 * @param mesh_data pointer to the character array containing the object file data
 * @returns true if the mesh was successfully loaded from the data, or false if something went wrong
 * **/
bool Mesh::readObj(const char* mesh_data)
{
    // lay a header over the start of the mesh data like a stencil
    file::BinaryMeshHeader* header;
    header = (file::BinaryMeshHeader*)mesh_data;
    
    // if the header is invalid, or there are no valid verts or tris, give up
    if (header->checksum != NOV_BINARY_MESH_HEADER_CHECKSUM 
     || header->vertex_buffer_length == 0 
     || header->triangle_buffer_length == 0) return false;

    // delete all existing data
    deallocateBuffers();

    // reallocate all the buffers
    vertices_count = header->vertex_buffer_length;
    vertices = new FVector3[vertices_count];
    vertices_capacity = vertices_count;

    triangles_count = header->triangle_buffer_length*3;
    triangles = new uint16_t[triangles_count];
    triangles_capacity = triangles_count;

    material_indices = new uint16_t[header->triangle_buffer_length];

    uvs = new FVector2[triangles_count];
    vertex_normals = new FVector3[triangles_count];

    // TODO: materials?

    // iterate over the vertices and read them into the mesh vertex buffer
    file::BinaryMeshVertex* verts = (file::BinaryMeshVertex*)((char*)header + header->vertex_buffer_offset);
    for (uint32_t i = 0; i < header->vertex_buffer_length; i++)
    {
        vertices[i].x = verts[i].x;
        vertices[i].y = verts[i].y;
        vertices[i].z = verts[i].z;
    }

    // iterate over triangles and read them into the triangle buffer, and also the uvs at the same time
    file::BinaryMeshTriangle* tris = (file::BinaryMeshTriangle*)((char*)header + header->triangle_buffer_offset);
    file::BinaryMeshNormal* norms = (file::BinaryMeshNormal*)((char*)header + header->normal_buffer_offset);
    file::BinaryMeshUV* coords = (file::BinaryMeshUV*)((char*)header + header->uv_buffer_offset);
    uint32_t i3 = 0;
    for (uint32_t i = 0; i < header->triangle_buffer_length; i++)
    {
        triangles[i3] = tris[i].v0;
        vertex_normals[i3] = ((tris[i].flags & NOV_BINARY_MESH_TRIANGLE_N0) 
                            && (tris[i].n0 < header->normal_buffer_length))
                            ? FVector3{ norms[tris[i].n0].x, norms[tris[i].n0].y, norms[tris[i].n0].z }
                            : FVector3{ 0.0f, 0.0f, 0.0f };
        uvs[i3] = ((tris[i].flags & NOV_BINARY_MESH_TRIANGLE_U0) 
                 && (tris[i].u0 < header->uv_buffer_length))
                 ? FVector2{ coords[tris[i].u0].u, coords[tris[i].u0].v }
                 : FVector2{ 0.0f, 0.0f };
        i3++;
        triangles[i3] = tris[i].v1;
        vertex_normals[i3] = ((tris[i].flags & NOV_BINARY_MESH_TRIANGLE_N1) 
                            && (tris[i].n1 < header->normal_buffer_length))
                            ? FVector3{ norms[tris[i].n1].x, norms[tris[i].n1].y, norms[tris[i].n1].z }
                            : FVector3{ 0.0f, 0.0f, 0.0f };
        uvs[i3] = ((tris[i].flags & NOV_BINARY_MESH_TRIANGLE_U1) 
                 && (tris[i].u1 < header->uv_buffer_length))
                 ? FVector2{ coords[tris[i].u1].u, coords[tris[i].u1].v }
                 : FVector2{ 0.0f, 0.0f };
        i3++;
        triangles[i3] = tris[i].v2;
        vertex_normals[i3] = ((tris[i].flags & NOV_BINARY_MESH_TRIANGLE_N2) 
                            && (tris[i].n2 < header->normal_buffer_length))
                            ? FVector3{ norms[tris[i].n2].x, norms[tris[i].n2].y, norms[tris[i].n2].z }
                            : FVector3{ 0.0f, 0.0f, 0.0f };
        uvs[i3] = ((tris[i].flags & NOV_BINARY_MESH_TRIANGLE_U2) 
                 && (tris[i].u2 < header->uv_buffer_length))
                 ? FVector2{ coords[tris[i].u2].u, coords[tris[i].u2].v }
                 : FVector2{ 0.0f, 0.0f };
        i3++;
    }

    // pump the last bit of backing data, FIXME: disabled for now since most of the data isn't needed 
    updateMeshData();

    serial::com_1 << "successfully read a mesh with " << stream::Mode::DEC 
          << countVertices() << " verts and " 
          << countTriangles() << " tris." << stream::endl;

    // we're done
    return true;
}

uint16_t Mesh::countVertices() { return vertices_count; }
uint16_t Mesh::countTriangles() { return triangles_count / 3; }

Mesh::Mesh() { }
Mesh::Mesh(const char* obj_data) { readObj(obj_data); }

Mesh::~Mesh()
{
    deallocateBuffers();
}

void boundsMinMaxFromCenterRadius(Bounds& bounds)
{
    bounds.min = bounds.center - bounds.radius;
    bounds.max = bounds.center + bounds.radius;
}

void boundsCenterRadiusFromMinMax(Bounds& bounds)
{
    boundsFixMinmax(bounds);
    bounds.radius = (bounds.max - bounds.min) * 0.5f;
    bounds.center = bounds.min + bounds.radius;
}

void boundsFixMinmax(Bounds& bounds)
{
    float tmp = 0.0f;
    if (bounds.min.x > bounds.max.x) { tmp = bounds.min.x; bounds.min.x = bounds.max.x; bounds.max.x = tmp; }
    if (bounds.min.y > bounds.max.y) { tmp = bounds.min.y; bounds.min.y = bounds.max.y; bounds.max.y = tmp; }
    if (bounds.min.z > bounds.max.z) { tmp = bounds.min.z; bounds.min.z = bounds.max.z; bounds.max.z = tmp; }
}

void drawCharacter(char chr, const UVector2& origin, const Colour& col, const Font& font, const Framebuffer& framebuffer)
{
    uint8_t glyph_base_x = (chr % font.tiles_per_row);
    uint8_t glyph_base_y = (chr / font.tiles_per_row);
    uint8_t* glyph_start = font.bitmap + ((glyph_base_x + (glyph_base_y * font.char_height * font.tiles_per_row)) * font.char_width);

    // TODO: small efficiency improvement here
    for (uint8_t y = 0; y < font.char_height; y++)
    {
        for (uint8_t x = 0; x < font.char_width; x++)
        {
            if (glyph_start[x + (y * font.bitmap_width)])
            {
                UVector2 position = origin + UVector2{ x,y };
                setPixel(position.u + (position.v * framebuffer.size.u), col, framebuffer);
            }
        }
    }
}

}
}