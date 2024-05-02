#include <boot.h>
#include <serial.h>
#include <memory.h>
#include <gui.h>
#include <panic.h>
#include <3d_demo_meshes.h>
#include <array.h>
#include <string.h>

// TODO: string splitting
// TODO: textbox panel
// TODO: interrupts
// TODO: keyboard
// TODO: v-tables
// TODO: timing

using namespace nov;
using namespace stream;

extern "C" void main(boot::nov_os_hint_table* os_hint_table)
{
    init_serial(COM1);
    com_1 << "hello from kernel main." << endl;
    com_1.flush();
    com_1 << "reading the os hint table..." << endl;
    com_1 << "    gdt address      : " << mode::HEX << (uint32_t)os_hint_table->gdt_address << endl;
    com_1 << "    1kib low blocks  : " << mode::DEC << os_hint_table->low_kilobyte_blocks << endl;
    com_1 << "    CPUID ecx        : " << mode::BIN << os_hint_table->cpuid_ecx_content << endl;
    com_1 << "    CPUID edx        : " << mode::BIN << os_hint_table->cpuid_edx_content << endl;
    com_1 << "    memory map table : " << mode::HEX << (uint32_t)os_hint_table->memory_map_table_address << endl;
    com_1 << "    table entries    : " << mode::DEC << os_hint_table->memory_map_table_entries << endl;
    com_1 << "    boot disk        : " << mode::DEC << os_hint_table->boot_disk_number << endl;
    com_1 << "    BDA address      : " << mode::HEX << (uint32_t)os_hint_table->bios_data_area_address << endl;
    com_1 << "    kernel elf start : " << mode::HEX << (uint32_t)os_hint_table->kernel_elf_start << endl;
    com_1 << "    kernel elf end   : " << mode::HEX << (uint32_t)os_hint_table->kernel_elf_end << endl;
    com_1 << "    kernel elf size  : " << mode::DEC << (uint32_t)os_hint_table->kernel_elf_end-(uint32_t)os_hint_table->kernel_elf_start << endl;
    com_1 << "    checksum         : " << mode::HEX << os_hint_table->checksum << endl;
    com_1 << "    novo             : " << mode::HEX << os_hint_table->checksum_novo << endl;
    com_1.flush();

    com_1 << endl;

    com_1 << "reading high memory map..." << endl;

    for (int m = 0; m < os_hint_table->memory_map_table_entries; m++)
    {
        com_1 << "memory map entry " << mode::DEC << m << ':' << endl;

        uint64_t size = os_hint_table->memory_map_table_address[m].region_size;
        const char* size_scale = "B)\0\0";
        if (size > 4096) { size /= 1024; size_scale = "KiB)"; }
        if (size > 4096) { size /= 1024; size_scale = "MiB)"; }
        if (size > 4096) { size /= 1024; size_scale = "GiB)"; }

        com_1 << "    base address low   : " << mode::HEX << (uint32_t)(os_hint_table->memory_map_table_address[m].region_base & 0xFFFFFFFF) << endl;
        com_1 << "    base address high  : " << mode::HEX << (uint32_t)(os_hint_table->memory_map_table_address[m].region_base >> 32) << endl;
        com_1 << "    region size low    : " << mode::HEX << (uint32_t)(os_hint_table->memory_map_table_address[m].region_size & 0xFFFFFFFF) << endl;
        com_1 << "    region size high   : " << mode::HEX << (uint32_t)(os_hint_table->memory_map_table_address[m].region_size >> 32);
        com_1 << " (" << mode::DEC << (uint32_t)size << size_scale << endl;
        com_1 << "    region type        : " << mode::DEC << (os_hint_table->memory_map_table_address[m].region_type == 1 ? "free" : "reserved") << endl;
        com_1 << "    acpi data          : " << mode::BIN << os_hint_table->memory_map_table_address[m].region_acpi << endl;
    }

    com_1 << endl;
    com_1.flush();

    com_1 << "configuring memory manager" << endl;
    uint8_t selected_map_entry = 3;
    // FIXME: this makes big assumptions (32/64 bit and also memory map entry order)
    uint32_t mmap_start = os_hint_table->memory_map_table_address[selected_map_entry].region_base;
    uint32_t mmap_size = os_hint_table->memory_map_table_address[selected_map_entry].region_size;

    uint32_t kernel_size = (uint32_t)os_hint_table->kernel_elf_end-(uint32_t)os_hint_table->kernel_elf_start;
    
    memory::minit((void*)(mmap_start+kernel_size), (mmap_size-kernel_size));
    
    uint8_t* real_buffer = (uint8_t*)os_hint_table->vbe_mode_info_block->flat_framebuffer_address;
    uint8_t* backbuffer = new uint8_t[640*480*3];
    if (backbuffer == 0x0) { com_1 << "unable to allocate memory for GUI backbuffer. panic!" << endl; panic(); }
    memory::mview();

    graphics::nov_framebuffer framebuffer{ backbuffer, nov_uvector2{ 640, 480 }, 3 };
    gui::nov_gui_manager man (framebuffer);

    auto root = man.get_root();
    auto text_panel = new gui::nov_panel_textbox();
    root->panel = text_panel;

    text_panel->text = "The quick brown fox jumps over the lazy dog. Lorem ipsum dolor sit amet.";
    text_panel->text_colour = nov_colour{ 255, 255, 255 };

    man.draw_root();
    memory::memcpy((uint32_t*)backbuffer, (uint32_t*)real_buffer, 640*120*3);

    com_1 << "all done." << endl;
    com_1.flush();

    while(true);
}