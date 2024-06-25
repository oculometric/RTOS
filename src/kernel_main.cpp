#include <boot.h>
#include <serial.h>
#include <interrupts.h>
#include <exceptions.h>
#include <memory.h>
#include <panic.h>
#include <gui.h>
#include <binary_bitmap.h>
#include <binary_mesh.h>
#include <3d_demo_meshes.h>
#include <keyboard.h>
#include <timer.h>
#include <gdt.h>

// TODO: string splitting
// TODO: v-tables
// TODO: timing
// TODO: text wrapping

using namespace nov;
using namespace stream;

extern "C" void main(boot::OSHintTable* os_hint_table)
{
    serial::initSerial(serial::Port::COM1);
    serial::com_1 << "hello from kernel main." << endl;
    serial::com_1.flush();
    serial::com_1 << "reading the os hint table..." << endl;
    serial::com_1 << "    gdt address      : " << Mode::HEX << (uint32_t)os_hint_table->gdt_address << endl;
    serial::com_1 << "    1kib low blocks  : " << Mode::DEC << os_hint_table->low_kilobyte_blocks << endl;
    serial::com_1 << "    CPUID ecx        : " << Mode::BIN << os_hint_table->cpuid_ecx_content << endl;
    serial::com_1 << "    CPUID edx        : " << Mode::BIN << os_hint_table->cpuid_edx_content << endl;
    serial::com_1 << "    memory map table : " << Mode::HEX << (uint32_t)os_hint_table->memory_map_table_address << endl;
    serial::com_1 << "    table entries    : " << Mode::DEC << os_hint_table->memory_map_table_entries << endl;
    serial::com_1 << "    boot disk        : " << Mode::DEC << os_hint_table->boot_disk_number << endl;
    serial::com_1 << "    BDA address      : " << Mode::HEX << (uint32_t)os_hint_table->bios_data_area_address << endl;
    serial::com_1 << "    kernel elf start : " << Mode::HEX << (uint32_t)os_hint_table->kernel_elf_start << endl;
    serial::com_1 << "    kernel elf end   : " << Mode::HEX << (uint32_t)os_hint_table->kernel_elf_end << endl;
    serial::com_1 << "    kernel elf size  : " << Mode::DEC << (uint32_t)os_hint_table->kernel_elf_end-(uint32_t)os_hint_table->kernel_elf_start << endl;
    serial::com_1 << "    checksum         : " << Mode::HEX << os_hint_table->checksum << endl;
    serial::com_1 << "    novo             : " << Mode::HEX << os_hint_table->checksum_novo << endl;
    serial::com_1.flush();

    serial::com_1 << endl;

    serial::com_1 << "reading high memory map..." << endl;

    for (int m = 0; m < os_hint_table->memory_map_table_entries; m++)
    {
        serial::com_1 << "memory map entry " << Mode::DEC << m << ':' << endl;

        uint64_t size = os_hint_table->memory_map_table_address[m].region_size;
        const char* size_scale = "B)\0\0";
        if (size > 4096) { size /= 1024; size_scale = "KiB)"; }
        if (size > 4096) { size /= 1024; size_scale = "MiB)"; }
        if (size > 4096) { size /= 1024; size_scale = "GiB)"; }

        serial::com_1 << "    base address low   : " << Mode::HEX << (uint32_t)(os_hint_table->memory_map_table_address[m].region_base & 0xFFFFFFFF) << endl;
        serial::com_1 << "    base address high  : " << Mode::HEX << (uint32_t)(os_hint_table->memory_map_table_address[m].region_base >> 32) << endl;
        serial::com_1 << "    region size low    : " << Mode::HEX << (uint32_t)(os_hint_table->memory_map_table_address[m].region_size & 0xFFFFFFFF) << endl;
        serial::com_1 << "    region size high   : " << Mode::HEX << (uint32_t)(os_hint_table->memory_map_table_address[m].region_size >> 32);
        serial::com_1 << " (" << Mode::DEC << (uint32_t)size << size_scale << endl;
        serial::com_1 << "    region type        : " << Mode::DEC << (os_hint_table->memory_map_table_address[m].region_type == 1 ? "free" : "reserved") << endl;
        serial::com_1 << "    acpi data          : " << Mode::BIN << os_hint_table->memory_map_table_address[m].region_acpi << endl;
    }

    serial::com_1 << endl;
    serial::com_1.flush();

    serial::com_1 << "configuring memory manager" << endl;
    uint8_t selected_map_entry = 3;
    // FIXME: this makes big assumptions (32/64 bit and also memory map entry order)
    uint32_t mmap_start = os_hint_table->memory_map_table_address[selected_map_entry].region_base;
    uint32_t mmap_size = os_hint_table->memory_map_table_address[selected_map_entry].region_size;

    uint32_t kernel_size = (uint32_t)os_hint_table->kernel_elf_end-(uint32_t)os_hint_table->kernel_elf_start;
    
    memory::mInit((void*)(mmap_start+kernel_size), (mmap_size-kernel_size));

    disableInterrupts();
    gdt::ProtectedGDTEntry new_gdt[3];
    // null entry
    new_gdt[0] = gdt::ProtectedGDTEntry{ 0, 0, 0, 0, 0, 0 };
    // kernel code entry
    new_gdt[1] = gdt::createProtectedGDTEntry(0x0, 0xFFFFF, Privilege::LEVEL_0, gdt::SegmentConfig::CODE_NONCONFORMING_READABLE, gdt::Flags::PROT_MODE_PAGE);
    // kernel data entry
    new_gdt[2] = gdt::createProtectedGDTEntry(0x0, 0xFFFFF, Privilege::LEVEL_0, gdt::SegmentConfig::DATA_UPWARD_WRITEABLE, gdt::Flags::PROT_MODE_PAGE);
    // jettison old bootloader GDT
    gdt::loadGDT(new_gdt, 3, 1, 2);
    serial::com_1 << "new GDT located at " << Mode::HEX << (uint32_t)new_gdt << endl;
    // TODO: task segment
    
    //uint8_t* real_buffer = (uint8_t*)os_hint_table->vbe_mode_info_block->flat_framebuffer_address;
    //uint8_t* backbuffer = new uint8_t[640*480*3];
    //if (backbuffer == 0x0) panic("unable to allocate memory for GUI backbuffer");
    memory::mView();

    serial::com_1 << "configuring IDT" << endl;
    interrupts::configureIDT(1);
    exception::registerExceptionHandlers();
    interrupts::configureIRQs((uint8_t)0x20);
    enableInterrupts();
    interrupts::configureIRQHandler(0, timer::timerInterruptCallback);
    interrupts::configureIRQHandler(1, keyboard::keyboardInterruptCallback);
    interrupts::setIRQEnabled(0, true);
    interrupts::setIRQEnabled(1, true);
    serial::com_1 << "okidoke, all setup." << endl;

    serial::com_1 << "configuring keyboard" << endl;
    keyboard::KeyboardDriver* keyboard_driver = new keyboard::KeyboardDriver();
    keyboard::assignKeyboardDriver(keyboard_driver);

    //graphics::Framebuffer framebuffer{ backbuffer, UVector2{ 640, 480 }, 3 };
    //gui::GuiManager man (framebuffer);

    file::BinaryBitmapHeader* font_header = (file::BinaryBitmapHeader*)_res_font_binbmp_start;
    serial::com_1 << "font checksum:        " << font_header->checksum << endl;
    serial::com_1 << "bitmap checksum:      " << NOV_BINARY_BITMAP_HEADER_CHECKSUM << endl;
    serial::com_1 << Mode::DEC;
    serial::com_1 << "font bitmap width:    " << font_header->image_width << endl;
    serial::com_1 << "font bitmap height:   " << font_header->image_height << endl;
    serial::com_1 << "font bits per pixel:  " << font_header->bits_per_pixel << endl;
    serial::com_1 << "font bitmap length:   " << font_header->image_size << endl;
    serial::com_1 << "font bitmap offset:   " << font_header->data_offset << endl;
    serial::com_1.flush();

    Font* font = new Font();
    font->char_width = 5;
    font->char_height = 8;
    font->bitmap = ((uint8_t*)font_header + font_header->data_offset);
    font->bitmap_width = font_header->image_width;
    font->bitmap_height = font_header->image_height;
    font->tiles_per_row = font->bitmap_width / font->char_width;
    font->tiles_per_column = font->bitmap_height / font->char_height;

    /*
    man.guiFont = font;

    auto root = man.getRoot();

    auto text_panel = new gui::PanelTextbox();
    text_panel->name ="text panel";
    text_panel->font = font;
    text_panel->text = R"""(here is some text, this string is multiline)""";

    auto star_panel = new gui::PanelStar();
    star_panel->name = "star";
    star_panel->foreground = nov_colour_gold;

    auto mmap_panel = new gui::PanelMemoryMonitor();
    mmap_panel->name = "memory";

    auto mesh_panel = new gui::PanelMeshrender();
    mesh_panel->name = "teapot";
    mesh_panel->line_colour = nov_colour_indigo;
    mesh_panel->camera_up_direction = norm(FVector3{1,0,1});
    mesh_panel->camera_look_direction = norm(FVector3{1,0,-1});
    mesh_panel->camera_position = FVector3{-4,0,4};
    mesh_panel->mesh = new graphics::Mesh(_res_teapot_binmesh_start);

    gui::splitContainer(root, FVector2{0, 0.8f});
    auto top_container = root->child_a;
    auto bottom_container = root->child_b;
    gui::splitContainer(top_container, FVector2{0.8f, 0});
    top_container->child_b->panel = star_panel;
    gui::splitContainer(bottom_container, FVector2{0.3f, 0});
    bottom_container->child_a->panel = mmap_panel;
    bottom_container->child_b->panel = text_panel;
    gui::splitContainer(top_container->child_b, FVector2{0, 0.7f});
    top_container->child_b->child_b->panel = mesh_panel;
    
    text_panel->text_colour = nov_colour_carmine;
    serial::com_1 << "string is: \"" << text_panel->text << "\"" << endl;
    */
    //man.drawRoot();

    gui::Compositor compositor((uint8_t*)os_hint_table->vbe_mode_info_block->flat_framebuffer_address, 640, 480, font);

    while (true)
    {
        //man.drawSpecific(bottom_container->child_b);
        //memory::memCpy((uint32_t*)backbuffer, (uint32_t*)real_buffer, 640*120*3);

        if (keyboard_driver->hasEventWaiting())
        {
            keyboard::KeyEvent event = keyboard_driver->pollNextEvent();
            if (event.is_down)
            {
                if (event.ascii)
                {
                    //if (event.ascii == '\b') text_panel->text.pop();
                    //else text_panel->text.append(event.ascii);
                }
            }
        }
    }

    serial::com_1 << "all done." << endl;
    serial::com_1.flush();

    while(true);
}