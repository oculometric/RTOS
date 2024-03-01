#include <boot.h>
#include <serial.h>
#include <itos.h>
#include <vbe.h>
#include <graphics.h>
#include <colour.h>
#include <memory.h>
#include <gui/gui.h>
#include <gui/panel_graphicsdemos.h>
#include <gui/panel_textbox.h>
#include <gui/panel_memorymonitor.h>
#include <random.h>
#include <array.h>

// TODO: keyboard
// TODO: interrupts
// TODO: output streams
// TODO: timing
// TODO: textbox panel

using namespace nov;

extern "C" void main(boot::nov_os_hint_table* os_hint_table)
{
    init_serial(COM1);
    serial_println((char*)"hello from kernel main.", COM1);
    serial_println((char*)"reading the os hint table...", COM1);
    serial_print((char*)"   gdt address     : ", COM1); serial_println_hex((uint32_t)os_hint_table->gdt_address, COM1);
    serial_print((char*)"   1kib lows blocks: ", COM1); serial_println_dec(os_hint_table->low_kilobyte_blocks, COM1);
    serial_print((char*)"   CPUID ecx       : ", COM1); serial_println_bin(os_hint_table->cpuid_ecx_content, COM1);
    serial_print((char*)"   CPUID edx       : ", COM1); serial_println_bin(os_hint_table->cpuid_edx_content, COM1);
    serial_print((char*)"   memory map table: ", COM1); serial_println_hex((uint32_t)os_hint_table->memory_map_table_address, COM1);
    serial_print((char*)"   table entries   : ", COM1); serial_println_dec(os_hint_table->memory_map_table_entries, COM1);
    serial_print((char*)"   boot disk       : ", COM1); serial_println_dec(os_hint_table->boot_disk_number, COM1);
    serial_print((char*)"   BDA address     : ", COM1); serial_println_hex((uint32_t)os_hint_table->bios_data_area_address, COM1);
    serial_print((char*)"   kernel elf start: ", COM1); serial_println_hex((uint32_t)os_hint_table->kernel_elf_start, COM1);
    serial_print((char*)"   kernel elf end  : ", COM1); serial_println_hex((uint32_t)os_hint_table->kernel_elf_end, COM1);
    serial_print((char*)"   kernel elf size : ", COM1); serial_println_dec((uint32_t)os_hint_table->kernel_elf_end-(uint32_t)os_hint_table->kernel_elf_start, COM1);
    serial_print((char*)"   checksum        : ", COM1); serial_println_hex((uint32_t)os_hint_table->checksum, COM1);
    serial_print((char*)"   novo checksum   : ", COM1); serial_println_hex((uint32_t)os_hint_table->checksum_novo, COM1);

    serial_println(COM1);

    serial_println((char*)"reading high memory map...", COM1);

    for (int m = 0; m < os_hint_table->memory_map_table_entries; m++)
    {
        serial_print((char*)"memory map entry ", COM1); serial_print_dec(m, COM1); serial_println((char*)":", COM1);

        uint64_t size = os_hint_table->memory_map_table_address[m].region_size;
        const char* size_scale = "B)\0\0";
        if (size > 4096) { size /= 1024; size_scale = "KiB)"; }
        if (size > 4096) { size /= 1024; size_scale = "MiB)"; }
        if (size > 4096) { size /= 1024; size_scale = "GiB)"; }

        serial_print((char*)"   base address low  : ", COM1); serial_println_hex(os_hint_table->memory_map_table_address[m].region_base & 0xFFFFFFFF, COM1);
        serial_print((char*)"   base address high : ", COM1); serial_println_hex(os_hint_table->memory_map_table_address[m].region_base >> 32, COM1);
        serial_print((char*)"   region size low   : ", COM1); serial_println_hex(os_hint_table->memory_map_table_address[m].region_size & 0xFFFFFFFF, COM1);
        serial_print((char*)"   region size high  : ", COM1); serial_print_hex(os_hint_table->memory_map_table_address[m].region_size >> 32, COM1);
        serial_print((char*)" (", COM1); serial_print_dec(size, COM1); serial_println((char*)size_scale, COM1);
        serial_print((char*)"   region type       : ", COM1); serial_println_dec(os_hint_table->memory_map_table_address[m].region_type, COM1);
        serial_print((char*)"   acpi data         : ", COM1); serial_println_bin(os_hint_table->memory_map_table_address[m].region_acpi, COM1);
    }

    serial_println(COM1);

    serial_println((char*)"configuring memory manager", COM1);
    uint8_t selected_map_entry = 3;
    // FIXME: this makes big assumptions (32/64 bit and also memory map entry order)
    uint32_t mmap_start = os_hint_table->memory_map_table_address[selected_map_entry].region_base;
    uint32_t mmap_size = os_hint_table->memory_map_table_address[selected_map_entry].region_size;

    uint32_t kernel_size = (uint32_t)os_hint_table->kernel_elf_end-(uint32_t)os_hint_table->kernel_elf_start;
    
    memory::minit((void*)(mmap_start+kernel_size), (mmap_size-kernel_size));
    
    uint8_t* real_buffer = (uint8_t*)os_hint_table->vbe_mode_info_block->flat_framebuffer_address;
    uint8_t* backbuffer = new uint8_t[640*480*3];
    if (backbuffer == 0x0) { serial_println((char*)"unable to allocate memory for GUI backbuffer. panic!", COM1); panic(); }
    memory::mview();

    graphics::nov_framebuffer framebuffer{ backbuffer, nov_uvector2{ 640, 480 }, 3 };
    gui::nov_gui_manager man (framebuffer);

    auto root = man.get_root();
    gui::split_container(root, nov_fvector2{ 0.7f, 0.0f });
    gui::split_container(root->child_a, nov_fvector2{ 0.0f, 0.8f });
    gui::split_container(root->child_a->child_b, nov_fvector2{ 0.25f, 0.0f });
    man.draw_root();
    memory::memcpy((uint32_t*)backbuffer, (uint32_t*)real_buffer, 640*120*3);

    gui::nov_panel_cuberender* pan_cube = new gui::nov_panel_cuberender();
    pan_cube->line_colour = nov_colour{ 1,1,1 };
    pan_cube->radius = nov_fvector3{ 1,1,1 };
    pan_cube->rotation = nov_fvector3{ 33,0, 45 };

    gui::nov_panel_star* pan_star = new gui::nov_panel_star();
    pan_star->background = nov_colour_nearblack;

    gui::nov_panel_textbox* pan_text = new gui::nov_panel_textbox();
    pan_text->text_colour = nov_colour_red;
    pan_text->text = (char*)"Hello, World!";

    gui::nov_panel_memorymonitor* pan_mem = new gui::nov_panel_memorymonitor();

    root->child_a->child_a->panel = pan_cube;
    root->child_a->child_b->child_b->panel = pan_text;
    root->child_a->child_b->child_a->panel = pan_mem;
    root->child_b->panel = pan_star;

    memory::mview();
    
    man.draw_root();
    memory::memcpy((uint32_t*)backbuffer, (uint32_t*)real_buffer, 640*120*3);

    serial_println((char*)"starting speedtest",COM1);
    bool x_increasing = false;
    bool y_increasing = false;
    for (uint32_t tmp = 0; tmp < 800; tmp++)
    {
        pan_star->foreground = nov_colour{ (uint8_t)(pan_star->uv.u * 255),(uint8_t)((1 - pan_star->uv.v) * 255), 21 };
        pan_star->uv.u += x_increasing ? 0.005f : -0.005f;
        pan_star->uv.v += y_increasing ? 0.02f : -0.02f;
        if (pan_star->uv.u <= 0.005f) x_increasing = true;
        if (pan_star->uv.u >= 1.0f) x_increasing = false;
        if (pan_star->uv.v <= 0.02f) y_increasing = true;
        if (pan_star->uv.v >= 1.0f) y_increasing = false;

        man.draw_specific(root->child_b);
        memory::memcpy((uint32_t*)backbuffer, (uint32_t*)real_buffer, 640*120*3);
    }
    serial_println((char*)"speedtest done",COM1);

    pan_star->foreground = nov_colour_gold;
    pan_star->uv = nov_fvector2{ 0.9, 0.9 };
    man.draw_root();
    memory::memcpy((uint32_t*)backbuffer, (uint32_t*)real_buffer, 640*120*3);

    // for (int s = 0; s < 10; s++)
    // {
    //     for (uint32_t t = 0; t < 102400000; t++) {}

    //     man.frame_outline_colour = nov_colour{ random_uint32_t() & 0xFF, random_uint32_t() & 0xFF, random_uint32_t() && 0xFF };
    //     float div;
    //     do
    //     {
    //         div = (float)random_uint32_t() / (float)0xFFFFFFFF;
    //     } while (div < 0.2f || div > 0.8f);

    //     gui::split_container(root, nov_fvector2{ s % 2 ? 0 : div, s % 2 ? div : 0 });
    //     man.draw_specific(root);
    //     memory::memcpy((uint32_t*)backbuffer, (uint32_t*)real_buffer, 640*120*3);
    //     root = random_bool() ? root->child_b : root->child_a;
    // }

    return;
}