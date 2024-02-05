#include <boot.h>
#include <serial.h>
#include <itos.h>
#include <vbe.h>
#include <graphics.h>
#include <colour.h>
#include <memory.h>
#include <window.h>
#include <random.h>

// TODO: keyboard
// TODO: interrupts
// TODO: memory stuff (memcpy, etc)
// TODO: output streams

void describe_memory_map()
{
    nov_memory_frame* current_block = head_frame;
    nov_memory_frame* next_block;
    uint32_t block_size;

    serial_println((char*)"=== MMAP START ===", COM1);
    while (current_block != 0x0)
    {
        next_block = current_block->next;
        if (next_block == 0x0) { serial_print((char*)"end block found at ", COM1); serial_println_hex((uint32_t)current_block, COM1); return; }

        block_size = (uint32_t)next_block-(uint32_t)current_block;
        serial_print((char*)"block at     ", COM1); serial_println_hex((uint32_t)current_block, COM1);
        serial_print((char*)"   size w/h  ", COM1); serial_print_dec(block_size, COM1); serial_print((char*)"/", COM1); serial_println_hex(block_size, COM1);
        serial_print((char*)"   size wo/h ", COM1); serial_print_dec(block_size-sizeof(nov_memory_frame), COM1); serial_print((char*)"/", COM1); serial_println_hex(block_size-sizeof(nov_memory_frame), COM1);
        serial_print((char*)"   next      ", COM1); serial_println_hex((uint32_t)current_block->next, COM1);
        serial_print((char*)"   is free?  ", COM1); serial_println_dec(current_block->is_free, COM1);
    
        current_block = next_block;
    }
    serial_println("=== MMAP END ===", COM1);
}


extern "C" void main(os_hint_table* os_hint_table_address)
{
    init_serial(COM1);
    serial_println((char*)"hello from kernel main.", COM1);
    serial_println((char*)"reading the os hint table...", COM1);
    serial_print((char*)"   gdt address     : ", COM1); serial_println_hex((uint32_t)os_hint_table_address->gdt_address, COM1);
    serial_print((char*)"   1kib lows blocks: ", COM1); serial_println_dec(os_hint_table_address->low_kilobyte_blocks, COM1);
    serial_print((char*)"   CPUID ecx       : ", COM1); serial_println_bin(os_hint_table_address->cpuid_ecx_content, COM1);
    serial_print((char*)"   CPUID edx       : ", COM1); serial_println_bin(os_hint_table_address->cpuid_edx_content, COM1);
    serial_print((char*)"   memory map table: ", COM1); serial_println_hex((uint32_t)os_hint_table_address->memory_map_table_address, COM1);
    serial_print((char*)"   table entries   : ", COM1); serial_println_dec(os_hint_table_address->memory_map_table_entries, COM1);
    serial_print((char*)"   boot disk       : ", COM1); serial_println_dec(os_hint_table_address->boot_disk_number, COM1);
    serial_print((char*)"   BDA address     : ", COM1); serial_println_hex((uint32_t)os_hint_table_address->bios_data_area_address, COM1);
    serial_print((char*)"   kernel elf start: ", COM1); serial_println_hex((uint32_t)os_hint_table_address->kernel_elf_start, COM1);
    serial_print((char*)"   kernel elf end  : ", COM1); serial_println_hex((uint32_t)os_hint_table_address->kernel_elf_end, COM1);
    serial_print((char*)"   kernel elf size : ", COM1); serial_println_dec((uint32_t)os_hint_table_address->kernel_elf_end-(uint32_t)os_hint_table_address->kernel_elf_start, COM1);
    serial_print((char*)"   checksum        : ", COM1); serial_println_hex((uint32_t)os_hint_table_address->checksum, COM1);
    serial_print((char*)"   novo checksum   : ", COM1); serial_println_hex((uint32_t)os_hint_table_address->checksum_novo, COM1);

    serial_println(COM1);

    serial_println((char*)"reading high memory map...", COM1);

    for (int m = 0; m < os_hint_table_address->memory_map_table_entries; m++)
    {
        serial_print((char*)"memory map entry ", COM1); serial_print_dec(m, COM1); serial_println((char*)":", COM1);

        uint64_t size = os_hint_table_address->memory_map_table_address[m].region_size;
        const char* size_scale = "B)\0\0";
        if (size > 4096) { size /= 1024; size_scale = "KiB)"; }
        if (size > 4096) { size /= 1024; size_scale = "MiB)"; }
        if (size > 4096) { size /= 1024; size_scale = "GiB)"; }

        serial_print((char*)"   base address low  : ", COM1); serial_println_hex(os_hint_table_address->memory_map_table_address[m].region_base & 0xFFFFFFFF, COM1);
        serial_print((char*)"   base address high : ", COM1); serial_println_hex(os_hint_table_address->memory_map_table_address[m].region_base >> 32, COM1);
        serial_print((char*)"   region size low   : ", COM1); serial_println_hex(os_hint_table_address->memory_map_table_address[m].region_size & 0xFFFFFFFF, COM1);
        serial_print((char*)"   region size high  : ", COM1); serial_print_hex(os_hint_table_address->memory_map_table_address[m].region_size >> 32, COM1);
        serial_print((char*)" (", COM1); serial_print_dec(size, COM1); serial_println((char*)size_scale, COM1);
        serial_print((char*)"   region type       : ", COM1); serial_println_dec(os_hint_table_address->memory_map_table_address[m].region_type, COM1);
        serial_print((char*)"   acpi data         : ", COM1); serial_println_bin(os_hint_table_address->memory_map_table_address[m].region_acpi, COM1);
    }

    serial_println(COM1);

    serial_println((char*)"configuring memory manager", COM1);
    uint8_t selected_map_entry = 3;
    // FIXME: this makes big assumptions (32/64 bit and also memory map entry order)
    uint32_t mmap_start = os_hint_table_address->memory_map_table_address[selected_map_entry].region_base;
    uint32_t mmap_size = os_hint_table_address->memory_map_table_address[selected_map_entry].region_size;
    
    describe_memory_map();
    
    init_memory_manager((void*)mmap_start, mmap_size);

    describe_memory_map();
    void* kernel_address = malloc((uint32_t)os_hint_table_address->kernel_elf_end-(uint32_t)os_hint_table_address->kernel_elf_start);
    describe_memory_map();
    //serial_println_hex((uint32_t)kernel_address, COM1);

    //serial_println_hex((uint32_t)head_frame, COM1);
    //serial_println_hex((uint32_t)head_frame->next, COM1);

    void* my_memory_block = malloc (0xabcd);
    describe_memory_map();
    void* my_memory_other_block = malloc (0xff00);
    describe_memory_map();

    //serial_println_hex((uint32_t)head_frame->next->next, COM1);

    serial_println((char*)"woo!", COM1);

    mfree(my_memory_block);
    mfree(my_memory_other_block);
    describe_memory_map();

    mfree (0x0);
    describe_memory_map();

    mconsolidate();
    describe_memory_map();

    /*
    serial_println(COM1);
    serial_println((char*)"starting search for PMID block", COM1);
    void* result = find_pmid_block((void*)0, 0xFFFFFFF0);
    serial_println_hex((uint32_t)result, COM1);
    serial_println(COM1);
    serial_dump_hex_byte(result, 20, COM1, 4);

    serial_println(COM1);

    serial_println_dec(os_hint_table_address->vbe_mode_info_block->x_resolution, COM1);
    serial_println_dec(os_hint_table_address->vbe_mode_info_block->y_resolution, COM1);
    serial_println_hex(os_hint_table_address->vbe_mode_info_block->flat_framebuffer_address, COM1);

    
    */

    nov_graphics_manager g (os_hint_table_address->vbe_mode_info_block);

    g.draw_pixel(nov_uvector2{0,0}, nov_colour{1,0,0});

    g.draw_box(nov_uvector2{50,50}, nov_uvector2{200,200}, nov_colour_vapor);


    uint8_t* buffer_a = (uint8_t*)0x200000;
    uint8_t* buffer_b = (uint8_t*)0x300000;
    uint8_t* real_buffer = (uint8_t*)os_hint_table_address->vbe_mode_info_block->flat_framebuffer_address;

    nov_colour c;
    float l;
    uint32_t d = 0;
    for (int y = 0; y < 480; y++)
    {
        for (int x = 0; x < 640; x++)
        {
            c = hsv_to_rgb(nov_colour{x/640.0f,y/480.0f,1.0f});
            l = luminance(c);
            buffer_a[(d*3)+0] = (uint8_t)(l*255);
            buffer_a[(d*3)+1] = (uint8_t)(l*255);
            buffer_a[(d*3)+2] = (uint8_t)(l*255);

            buffer_b[(d*3)+0] = (uint8_t)(c.z*255);
            buffer_b[(d*3)+1] = (uint8_t)(c.y*255);
            buffer_b[(d*3)+2] = (uint8_t)(c.x*255);
            d++;
        }
    }


    serial_println(COM1);

    serial_println((char*)"testing screen", COM1);

    for (uint32_t t = 0; t < 10; t++)
    {
        memcpy((uint32_t*)buffer_a, (uint32_t*)real_buffer, 640*120*3);
        memcpy((uint32_t*)buffer_b, (uint32_t*)real_buffer, 640*120*3);
    }

    serial_println((char*)"done", COM1);

    serial_println((char*)"for my next trick, i will draw a window", COM1);
    draw_window(nov_uvector2{20, 40}, nov_uvector2{100, 100}, real_buffer, nov_uvector2{640,480});

    nov_uvector2 left {480,240};
    nov_uvector2 right {640, 0};
    nov_colour col = nov_colour_vapor;

    while (true)
    {
        g.draw_line(left, right, col);
        left.u = random_uint32_t() % 640;
        left.v = random_uint32_t() % 480;
        right.u = random_uint32_t() % 640;
        right.v = random_uint32_t() % 480;
        uint32_t r = random_uint32_t() & 0xFFFFF;
        col = nov_colour{ col.y, (float)r/0xFFFF, col.x };
        
        //for (uint32_t i = 0; i < 42000; i++) {}
    }

    return;
}