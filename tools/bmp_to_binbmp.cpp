#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>

#include "../src/include/binary_bitmap.h"

#pragma pack(push, 1)
struct BMPHeader
{
	uint8_t id_0 = 0;
	uint8_t id_1 = 0;

	uint32_t file_size = 0;

	uint16_t reserved_0 = 0;
	uint16_t reserved_1 = 0;

	uint32_t image_data_offset = 0;
};

enum BitmapInfoHeaderCompression
{
	BI_RGB = 0,
	BI_RLE8,
	BI_RLE4,
	BI_BITFIELDS,
	BI_JPEG,
	BI_PNG,
	BI_ALPHABITFIELDS,
	BI_CMYK = 11,
	BI_CMYKRLE8,
	BI_CMYKRLE4
};

struct BitmapInfoHeader
{
	uint32_t header_size = 40;

	uint32_t width_pixels;
	uint32_t height_pixels;

	uint16_t num_colour_planes;

	uint16_t num_bits_per_pixel;

	uint32_t compression_method;

	uint32_t image_size;

	uint32_t horizontal_resolution;
	uint32_t vertical_resolution;

	uint32_t num_palette_colours;

	uint32_t num_important_colours;
};
#pragma pack(pop)

using namespace std;
using namespace nov;
using namespace file;

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

    BMPHeader early_header;

    file.seekg(file.beg);
    file.read((char*)(&early_header), sizeof(early_header));
    if (early_header.id_0 != 'B' || early_header.id_1 != 'M') { file.close();  return 1; }

    uint32_t dib_header_size;
    file.read((char*)(&dib_header_size), 4);
    if (dib_header_size != 40) { file.close(); return 1; }

    BitmapInfoHeader bitmap_header;
    file.read((char*)(&bitmap_header) + 4, sizeof(bitmap_header) - 4);

    if (bitmap_header.compression_method != BI_RGB) { file.close(); return 1; }
    if (bitmap_header.num_colour_planes != 1) { file.close(); return 1; }

    bitmap_header.image_size = bitmap_header.width_pixels * bitmap_header.height_pixels * bitmap_header.num_bits_per_pixel / 8;

    cout << "width      : " << bitmap_header.width_pixels << endl;
    cout << "height     : " << bitmap_header.height_pixels << endl;
    cout << "length     : " << bitmap_header.image_size << endl;
    cout << "bpp        : " << bitmap_header.num_bits_per_pixel << endl;
    cout << "hres       : " << bitmap_header.horizontal_resolution << endl;
    cout << "vres       : " << bitmap_header.vertical_resolution << endl;

    uint8_t* pixel_data_bottom_to_top = new uint8_t[bitmap_header.image_size];
    file.seekg(file.beg + early_header.image_data_offset);
    file.read((char*)pixel_data_bottom_to_top, bitmap_header.image_size);

    file.close();

    uint8_t* pixel_data = new uint8_t[bitmap_header.image_size];
    uint32_t row_width = bitmap_header.width_pixels * bitmap_header.num_bits_per_pixel / 8;
    uint32_t pixel_index = 0;
    uint32_t pixel_index_bottom_up = row_width * (bitmap_header.height_pixels - 1);
    for (uint32_t y = 0; y < bitmap_header.height_pixels; y++)
    {
        for (uint32_t x = 0; x < row_width; x++)
        {
            pixel_data[pixel_index] = pixel_data_bottom_to_top[pixel_index_bottom_up];
            
            pixel_index++;
            pixel_index_bottom_up++;
        }
        pixel_index_bottom_up -= row_width * 2;
    }

    BinaryBitmapHeader header;
    header.checksum = NOV_BINARY_BITMAP_HEADER_CHECKSUM;
    header.data_offset = sizeof(BinaryBitmapHeader);
    header.bits_per_pixel = bitmap_header.num_bits_per_pixel;

    header.image_width = bitmap_header.width_pixels;
    header.image_height = bitmap_header.height_pixels;
    header.image_size = bitmap_header.image_size;

    cout << "success. outputting..." << endl;

    outfile.write((char*)(&header), sizeof(BinaryBitmapHeader));
    outfile.write((char*)pixel_data, header.image_size);
    outfile << ":)";

    outfile.close();

    cout << "done." << endl;
    return 0;
}