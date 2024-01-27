#include "vbe.h"

class novos_graphics_manager
{
private:
    uint8_t* real_framebuffer;
    uint16_t screen_size_x;
    uint16_t screen_size_y;
    uint8_t bytes_per_pixel;

public:
    novos_graphics_manager(vbe_mode_info* vbe_mode_info_block);

    draw_box()
};