#include <gui/desktop.h>

#include <binary_bitmap.h>
#include <3d_demo_meshes.h>
#include <keyboard.h>
#include <gui/interface.h>

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

    font = new Font();
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
            if (event.is_down
                && !is_overlay_showing
                && (event.modifiers & keyboard::Modifier::L_ALT) 
                && event.key != keyboard::K_L_ALT 
                && (event.key == keyboard::K_UP
                    || event.key == keyboard::K_DOWN
                    || event.key == keyboard::K_LEFT
                    || event.key == keyboard::K_RIGHT
                    )
                )
            {
                auto container = getActiveHandle();
                auto new_container = container;
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

                container.setTitle(intToString((uint32_t)container.getID(), 16));
                container.repaint();

                graphics::drawStar(new_container.getFramebuffer()->getWeakCopy(), UVector2{ 1,1 }, nov_colour_gold, nov_colour_nearblack, false);
                String title = intToString((uint32_t)new_container.getID(), 16);
                title.append(" (active)");
                new_container.setTitle(title);
                new_container.clear();
                new_container.blit();

                handle_order.insert(new_container, 0);
            }
            else if (event.is_down 
                && !is_overlay_showing
                && (event.key == keyboard::K_UP
                    || event.key == keyboard::K_DOWN
                    || event.key == keyboard::K_LEFT
                    || event.key == keyboard::K_RIGHT
                    )
                )
            {
                auto container = getActiveHandle();
                container.setTitle(intToString((uint32_t)container.getID(), 16));
                container.repaint();
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
                String title = intToString((uint32_t)container.getID(), 16);
                title.append(" (active)");
                container.setTitle(title);
                container.repaint();
                container.blit();

                handle_order.move(container, 0);
            }
            else if (!event.is_down && event.key == keyboard::K_L_GUI)
            {
                is_overlay_showing = !is_overlay_showing;
                primary_compositor->setOverlayVisible(is_overlay_showing);
            }
        }
    
        if (is_overlay_showing)
        {
            // TODO: immediate mode overlay
            ProtectedFramebuffer* fb = primary_compositor->getOverlayFramebuffer();
            graphics::fillBox(UVector2{ 0,0 }, fb->getSize(), nov_colour_magenta, fb->getWeakCopy());
            drawIMLabel("help me", IVector2{50,50}, IVector2{200, 20}, fb, font);
            primary_compositor->blitOverlayFramebuffer();
        }
    }
}

SegmentedDesktop::~SegmentedDesktop()
{
    delete primary_compositor;
}

}
}