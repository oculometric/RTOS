#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <memory.h>

#include "src/include/binary_mesh.h"

using namespace std;
using namespace nov;
using namespace file;

void split(const string line, vector<string> & split_line, const char delim)
{
    std::string section;
    split_line.clear();
    for (char c : line)
    {
        if (c == delim)
        {
            split_line.push_back(section);
            section.clear();
        }
        else
        {
            section += c;
        }
    }
    if (section != "") split_line.push_back(section);
}

int main(int argc, char* argv[])
{
    cout << "hello!" << endl;
    if (argc < 3) return 1;

    ifstream file;
    file.open(argv[1]);
    if (!file.is_open()) { cout << "failed to open input file." << endl; return 1; }
    ofstream outfile;
    outfile.open(argv[2], ios::binary);
    if (!outfile.is_open()) { cout << "failed to open output file." << endl; return 1; }

    cout << "input: " << argv[1] << endl;
    cout << "output: " << argv[2] << endl;

    uint16_t found_vertices = 0;
    uint16_t found_triangles = 0;
    uint16_t found_uvs = 0;
    uint16_t found_vnorms = 0;

    // prepass to count how many of each we need to allocate for
    string line;
    while(getline(file, line))
    {
        if (line.length() < 2) continue;
        if (line[0] == '#') continue;
        if (line[0] == 'v' && line[1] == ' ')
        {
            found_vertices++;
        }
        if (line[0] == 'f' && line[1] == ' ')
        {
            found_triangles++;
        }
        if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
        {
            found_uvs++;
        }
        if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
        {
            found_vnorms++;
        }
    }

    cout << "found: " << endl;
    cout << "   verts     : " << found_vertices << endl;
    cout << "   triangles : " << found_triangles << endl;
    cout << "   uvs       : " << found_uvs << endl;
    cout << "   vnorms    : " << found_vnorms << endl;

    if (found_vertices == 0 || found_triangles == 0) { cout << "stopping." << endl; return 1; }

    // allocate space for data
    auto vertices = new nov_binary_mesh_vertex[found_vertices];
    auto triangles = new nov_binary_mesh_triangle[found_triangles];
    auto material_indices = new uint16_t[found_triangles];

    nov_binary_mesh_uv* uvs = new nov_binary_mesh_uv[found_uvs];
    nov_binary_mesh_vertex* vnorms = new nov_binary_mesh_vertex[found_vnorms];

    // process file for realsies, but ignoring faces for now
    file.clear();
    file.seekg(0, std::ios::beg);
    int v = 0;
    int vt = 0;
    int vn = 0;
    int f = 0;
    nov_binary_mesh_vertex tmp3;
    nov_binary_mesh_uv tmp2;
    vector<string> split_line;
    vector<string> split_section;
    while(getline(file, line))
    {
        if (line.length() < 2) continue;
        if (line[0] == '#') continue;
        if (line[0] == 'v' && line[1] == ' ')
        {   // read vertex data
            split(line, split_line, ' ');
            if (split_line.size() < 4) { v++; continue; }
            tmp3.x = stof(split_line[1]);
            tmp3.y = stof(split_line[2]);
            tmp3.z = stof(split_line[3]);
            vertices[v] = tmp3;
            v++;
        }
        else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
        {   // read uv data
            split(line, split_line, ' ');
            if (split_line.size() < 3) { vt++; continue; }
            tmp2.u = stof(split_line[1]);
            tmp2.v = stof(split_line[2]);
            uvs[vt] = tmp2;
            vt++;
        }
        else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
        {   // read vertex normal data
            split(line, split_line, ' ');
            if (split_line.size() < 4) { vn++; continue; }
            tmp3.x = stof(split_line[1]);
            tmp3.y = stof(split_line[2]);
            tmp3.z = stof(split_line[3]);

            float length = sqrt((tmp3.x * tmp3.x) + (tmp3.y * tmp3.y) + (tmp3.z * tmp3.z));
            tmp3.x = tmp3.x / length;
            tmp3.y = tmp3.y / length;
            tmp3.z = tmp3.z / length;

            vnorms[vn] = tmp3;
            vn++;
        }
    }

    // TODO: handle materials
    // TODO: triangulation

    nov_binary_mesh_header header;
    header.checksum = NOV_BINARY_MESH_HEADER_CHECKSUM;
    header.vertex_buffer_length = found_vertices;
    header.triangle_buffer_length = found_triangles;
    header.normal_buffer_length = found_vnorms;
    header.uv_buffer_length = found_uvs;

    // process faces
    file.clear();
    file.seekg(0, std::ios::beg);
    while(getline(file, line))
    {
        if (line.length() < 2) continue;
        if (line[0] == '#') continue;
        if (line[0] == 'f' && line[1] == ' ')
        {   // read face data. we only handle triangles here
            split(line, split_line, ' ');
            if (split_line.size() < 4) { f++; continue; }
            triangles[f].flags = 0;

            // process first vertex data
            split(split_line[1], split_section, '/');
            triangles[f].v0 = stoi(split_section[0])-1;
            if (split_section.size() > 1 && split_section[1] != "")
            {
                triangles[f].u0 = stoi(split_section[1]);
                triangles[f].flags |= NOV_BINARY_MESH_TRIANGLE_U0;
            }
            if (split_section.size() > 2 && split_section[2] != "")
            {
                triangles[f].n0 = stoi(split_section[2]);
                triangles[f].flags |= NOV_BINARY_MESH_TRIANGLE_N0;
            }

            // process second vertex data
            split(split_line[2], split_section, '/');
            triangles[f].v1 = stoi(split_section[0])-1;
            if (split_section.size() > 1 && split_section[1] != "")
            {
                triangles[f].u1 = stoi(split_section[1]);
                triangles[f].flags |= NOV_BINARY_MESH_TRIANGLE_U1;
            }
            if (split_section.size() > 2 && split_section[2] != "")
            {
                triangles[f].n1 = stoi(split_section[2]);
                triangles[f].flags |= NOV_BINARY_MESH_TRIANGLE_N1;
            }

            // process third vertex data
            split(split_line[3], split_section, '/');
            triangles[f].v2 = stoi(split_section[0])-1;
            if (split_section.size() > 1 && split_section[1] != "")
            {
                triangles[f].u2 = stoi(split_section[1]);
                triangles[f].flags |= NOV_BINARY_MESH_TRIANGLE_U2;
            }
            if (split_section.size() > 2)
            {
                triangles[f].n2 = stoi(split_section[2]);
                triangles[f].flags |= NOV_BINARY_MESH_TRIANGLE_N2;
            }

            f++;
        }
    }

    file.close();

    cout << "success. outputting..." << endl;

    uint32_t total_size = sizeof(nov_binary_mesh_header)
                       + (sizeof(nov_binary_mesh_vertex) * header.vertex_buffer_length)
                       + (sizeof(nov_binary_mesh_triangle) * header.triangle_buffer_length)
                       + (sizeof(nov_binary_mesh_normal) * header.normal_buffer_length)
                       + (sizeof(nov_binary_mesh_uv) * header.uv_buffer_length);

    char* output_buffer = new char[total_size];
    
    uint32_t offset = 0;
    offset += sizeof(nov_binary_mesh_header);
    offset += 4;
    offset = (offset >> 2) << 2;

    memcpy(output_buffer+offset, (char*)vertices, sizeof(nov_binary_mesh_vertex) * header.vertex_buffer_length);
    header.vertex_buffer_offset = offset;
    offset += sizeof(nov_binary_mesh_vertex) * header.vertex_buffer_length;
    offset += 4;
    offset = (offset >> 2) << 2;

    memcpy(output_buffer+offset, (char*)triangles, sizeof(nov_binary_mesh_triangle) * header.triangle_buffer_length);
    header.triangle_buffer_offset = offset;
    offset += sizeof(nov_binary_mesh_triangle) * header.triangle_buffer_length;
    offset += 4;
    offset = (offset >> 2) << 2;

    memcpy(output_buffer+offset, (char*)vnorms, sizeof(nov_binary_mesh_normal) * header.normal_buffer_length);
    header.normal_buffer_offset = offset;
    offset += sizeof(nov_binary_mesh_normal) * header.normal_buffer_length;
    offset += 4;
    offset = (offset >> 2) << 2;

    memcpy(output_buffer+offset, (char*)uvs, sizeof(nov_binary_mesh_uv) * header.uv_buffer_length);
    header.uv_buffer_offset = offset;
    offset += sizeof(nov_binary_mesh_uv) * header.uv_buffer_length;
    offset += 4;
    offset = (offset >> 2) << 2;

    memcpy(output_buffer, (char*)&header, sizeof(nov_binary_mesh_header));

    delete[] vertices;
    delete[] triangles;
    delete[] uvs;
    delete[] vnorms;
    delete[] material_indices;

    outfile.write(output_buffer, total_size);
    outfile << ":)";

    outfile.close();

    cout << "done." << endl;

    return 0;
}
