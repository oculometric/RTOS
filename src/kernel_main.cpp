#include <boot.h>
#include <serial.h>
#include <memory.h>
#include <gui.h>
#include <panic.h>
#include <3d_demo_meshes.h>
#include <array.h>
#include <string.h>
#include <binary_bitmap.h>
#include <font.h>

// TODO: string splitting
// TODO: interrupts
// TODO: keyboard
// TODO: v-tables
// TODO: timing

using namespace nov;
using namespace stream;

extern "C" void main(boot::OSHintTable* os_hint_table)
{
    initSerial(COM1);
    com_1 << "hello from kernel main." << endl;
    com_1.flush();
    com_1 << "reading the os hint table..." << endl;
    com_1 << "    gdt address      : " << Mode::HEX << (uint32_t)os_hint_table->gdt_address << endl;
    com_1 << "    1kib low blocks  : " << Mode::DEC << os_hint_table->low_kilobyte_blocks << endl;
    com_1 << "    CPUID ecx        : " << Mode::BIN << os_hint_table->cpuid_ecx_content << endl;
    com_1 << "    CPUID edx        : " << Mode::BIN << os_hint_table->cpuid_edx_content << endl;
    com_1 << "    memory map table : " << Mode::HEX << (uint32_t)os_hint_table->memory_map_table_address << endl;
    com_1 << "    table entries    : " << Mode::DEC << os_hint_table->memory_map_table_entries << endl;
    com_1 << "    boot disk        : " << Mode::DEC << os_hint_table->boot_disk_number << endl;
    com_1 << "    BDA address      : " << Mode::HEX << (uint32_t)os_hint_table->bios_data_area_address << endl;
    com_1 << "    kernel elf start : " << Mode::HEX << (uint32_t)os_hint_table->kernel_elf_start << endl;
    com_1 << "    kernel elf end   : " << Mode::HEX << (uint32_t)os_hint_table->kernel_elf_end << endl;
    com_1 << "    kernel elf size  : " << Mode::DEC << (uint32_t)os_hint_table->kernel_elf_end-(uint32_t)os_hint_table->kernel_elf_start << endl;
    com_1 << "    checksum         : " << Mode::HEX << os_hint_table->checksum << endl;
    com_1 << "    novo             : " << Mode::HEX << os_hint_table->checksum_novo << endl;
    com_1.flush();

    com_1 << endl;

    com_1 << "reading high memory map..." << endl;

    for (int m = 0; m < os_hint_table->memory_map_table_entries; m++)
    {
        com_1 << "memory map entry " << Mode::DEC << m << ':' << endl;

        uint64_t size = os_hint_table->memory_map_table_address[m].region_size;
        const char* size_scale = "B)\0\0";
        if (size > 4096) { size /= 1024; size_scale = "KiB)"; }
        if (size > 4096) { size /= 1024; size_scale = "MiB)"; }
        if (size > 4096) { size /= 1024; size_scale = "GiB)"; }

        com_1 << "    base address low   : " << Mode::HEX << (uint32_t)(os_hint_table->memory_map_table_address[m].region_base & 0xFFFFFFFF) << endl;
        com_1 << "    base address high  : " << Mode::HEX << (uint32_t)(os_hint_table->memory_map_table_address[m].region_base >> 32) << endl;
        com_1 << "    region size low    : " << Mode::HEX << (uint32_t)(os_hint_table->memory_map_table_address[m].region_size & 0xFFFFFFFF) << endl;
        com_1 << "    region size high   : " << Mode::HEX << (uint32_t)(os_hint_table->memory_map_table_address[m].region_size >> 32);
        com_1 << " (" << Mode::DEC << (uint32_t)size << size_scale << endl;
        com_1 << "    region type        : " << Mode::DEC << (os_hint_table->memory_map_table_address[m].region_type == 1 ? "free" : "reserved") << endl;
        com_1 << "    acpi data          : " << Mode::BIN << os_hint_table->memory_map_table_address[m].region_acpi << endl;
    }

    com_1 << endl;
    com_1.flush();

    com_1 << "configuring memory manager" << endl;
    uint8_t selected_map_entry = 3;
    // FIXME: this makes big assumptions (32/64 bit and also memory map entry order)
    uint32_t mmap_start = os_hint_table->memory_map_table_address[selected_map_entry].region_base;
    uint32_t mmap_size = os_hint_table->memory_map_table_address[selected_map_entry].region_size;

    uint32_t kernel_size = (uint32_t)os_hint_table->kernel_elf_end-(uint32_t)os_hint_table->kernel_elf_start;
    
    memory::mInit((void*)(mmap_start+kernel_size), (mmap_size-kernel_size));
    
    uint8_t* real_buffer = (uint8_t*)os_hint_table->vbe_mode_info_block->flat_framebuffer_address;
    uint8_t* backbuffer = new uint8_t[640*480*3];
    if (backbuffer == 0x0) { com_1 << "unable to allocate memory for GUI backbuffer. panic!" << endl; panic(); }
    memory::mView();

    graphics::Framebuffer framebuffer{ backbuffer, UVector2{ 640, 480 }, 3 };
    gui::GuiManager man (framebuffer);

    auto root = man.getRoot();
    auto text_panel = new gui::PanelTextbox();
    root->panel = text_panel;

    file::BinaryBitmapHeader* font_header = (file::BinaryBitmapHeader*)_res_font_binbmp_start;
    com_1 << "font checksum:        " << font_header->checksum << endl;
    com_1 << "bitmap checksum:      " << NOV_BINARY_BITMAP_HEADER_CHECKSUM << endl;
    com_1 << Mode::DEC;
    com_1 << "font bitmap width:    " << font_header->image_width << endl;
    com_1 << "font bitmap height:   " << font_header->image_height << endl;
    com_1 << "font bits per pixel:  " << font_header->bits_per_pixel << endl;
    com_1 << "font bitmap length:   " << font_header->image_size << endl;
    com_1 << "font bitmap offset:   " << font_header->data_offset << endl;
    com_1.flush();

    Font* font = new Font();
    font->char_width = 5;
    font->char_height = 8;
    font->bitmap = ((uint8_t*)font_header + font_header->data_offset);
    font->bitmap_width = font_header->image_width;
    font->bitmap_height = font_header->image_height;
    font->tiles_per_row = font->bitmap_width / font->char_width;
    font->tiles_per_column = font->bitmap_height / font->char_height;
    text_panel->font = font;

    text_panel->text = R"""(0: The quick brown fox jumps over the lazy dog. Lorem ipsum dolor sit amet.
Bee Movie
By Jerry Seinfeld

NARRATOR:
(Black screen with text; The sound of buzzing bees can be heard)
According to all known laws
of aviation,
 :
there is no way a bee
should be able to fly.
 :
Its wings are too small to get
its fat little body off the ground.
 :
The bee, of course, flies anyway
 :
because bees don't care
what humans think is impossible.
BARRY BENSON:
(Barry is picking out a shirt)
Yellow, black. Yellow, black.
Yellow, black. Yellow, black.
 :
Ooh, black and yellow!
Let's shake it up a little.
JANET BENSON:
Barry! Breakfast is ready!
BARRY:
Coming!
 :
Hang on a second.
(Barry uses his antenna like a phone)
 :
Hello?
ADAM FLAYMAN:

(Through phone)
- Barry?
BARRY:
- Adam?
ADAM:
- Can you believe this is happening?
BARRY:
- I can't. I'll pick you up.
(Barry flies down the stairs)
    )""";
    text_panel->text_colour = nov_colour_carmine;

    com_1 << "string is: \"" << text_panel->text << "\"" << endl;

    while (true)
    {
        man.drawRoot();
        memory::memCpy((uint32_t*)backbuffer, (uint32_t*)real_buffer, 640*120*3);
        text_panel->text[0]++;
    }

    com_1 << "all done." << endl;
    com_1.flush();

    while(true);
}