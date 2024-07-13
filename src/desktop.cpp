#include <gui/desktop.h>

#include <binary_bitmap.h>
#include <3d_demo_meshes.h>
#include <keyboard.h>

namespace nov
{
namespace gui
{

SegmentedDesktop::SegmentedDesktop(uint8_t* framebuffer_address, uint16_t width, uint16_t height)
{
    file::BinaryBitmapHeader* font_header = (file::BinaryBitmapHeader*)_res_font_binbmp_start;
    serial::com_1 << "font checksum:        " << font_header->checksum << stream::endl;
    serial::com_1 << "bitmap checksum:      " << NOV_BINARY_BITMAP_HEADER_CHECKSUM << stream::endl;
    serial::com_1 << stream::Mode::DEC;
    serial::com_1 << "font bitmap width:    " << font_header->image_width << stream::endl;
    serial::com_1 << "font bitmap height:   " << font_header->image_height << stream::endl;
    serial::com_1 << "font bits per pixel:  " << font_header->bits_per_pixel << stream::endl;
    serial::com_1 << "font bitmap length:   " << font_header->image_size << stream::endl;
    serial::com_1 << "font bitmap offset:   " << font_header->data_offset << stream::endl;
    serial::com_1.flush();

    Font* font = new Font();
    font->char_width = 5;
    font->char_height = 8;
    font->bitmap = ((uint8_t*)font_header + font_header->data_offset);
    font->bitmap_width = font_header->image_width;
    font->bitmap_height = font_header->image_height;
    font->tiles_per_row = font->bitmap_width / font->char_width;
    font->tiles_per_column = font->bitmap_height / font->char_height;

    primary_compositor = new Compositor(framebuffer_address, width, height, font);

    ContainerHandle root = primary_compositor->getRootContainer();
    handle_order.insert(root, 0);

    root.setTitle("welcome to segmented desktop");
    root.setDecoration(ContainerDecorationMode::BORDERED_WITH_TITLE);
    graphics::drawStar(root.getFramebuffer()->getWeakCopy(), UVector2{ 1,1 }, nov_colour_gold, nov_colour_nearblack, true);
    root.clear();
    root.blit();
}

void SegmentedDesktop::takeControl()
{
    while (true)
    {
        if (keyboard::keyboardHasEventWaiting())
        {
            keyboard::KeyEvent event = keyboard::keyboardPollNextEvent();
            if (event.is_down && event.modifiers & keyboard::Modifier::L_ALT && event.key != keyboard::K_L_ALT)
            {
                auto container = getActiveHandle();
                auto new_container = container;
                serial::com_1 << "splitting: " << container.getID() << stream::endl;
                switch (event.key)
                {
                case keyboard::K_UP:
                    new_container = primary_compositor->divideContainer(container, gui::ContainerSplitDecision::SPLIT_HORIZONTAL_TOP); break;
                case keyboard::K_DOWN:
                    new_container = primary_compositor->divideContainer(container, gui::ContainerSplitDecision::SPLIT_HORIZONTAL_BOTTOM); break;
                case keyboard::K_LEFT:
                    new_container = primary_compositor->divideContainer(container, gui::ContainerSplitDecision::SPLIT_VERTICAL_LEFT); break;
                case keyboard::K_RIGHT:
                    new_container = primary_compositor->divideContainer(container, gui::ContainerSplitDecision::SPLIT_VERTICAL_RIGHT); break;
                default: break;
                }

                serial::com_1 << "done, main container is now: " << new_container.getID() << stream::endl;

                container.setTitle(intToString((uint32_t)container.getID(), 16));
                container.clear();

                graphics::drawStar(new_container.getFramebuffer()->getWeakCopy(), UVector2{ 1,1 }, nov_colour_gold, nov_colour_nearblack, false);
                new_container.setTitle(intToString((uint32_t)new_container.getID(), 16));
                new_container.clear();

                new_container.blit();

                handle_order.insert(new_container, 0);
            }
            else if (event.is_down)
            {
                auto container = getActiveHandle();
                container.clear();
                switch (event.key)
                {
                case keyboard::K_UP:
                    container = primary_compositor->getNorthContainer(container); break;
                case keyboard::K_DOWN:
                    container = primary_compositor->getSouthContainer(container); break;
                case keyboard::K_LEFT:
                    container = primary_compositor->getWestContainer(container); break;
                case keyboard::K_RIGHT:
                    container = primary_compositor->getEastContainer(container); break;
                default: break;
                }
                
                graphics::drawStar(container.getFramebuffer()->getWeakCopy(), UVector2{ 1,1 }, nov_colour_gold, nov_colour_nearblack, false);
                container.blit();
                handle_order.move(container, 0);
            }
        }
    }
}

SegmentedDesktop::~SegmentedDesktop()
{
    delete primary_compositor;
}

}
}