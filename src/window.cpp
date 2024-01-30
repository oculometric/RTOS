#include <window.h>

#include <serial.h>

void draw_window(const nov_uvector2& origin, const nov_uvector2& size, uint8_t* buffer, const nov_uvector2& buffer_size)
{
    serial_dump_hex_byte((void*)colour_palette, 64, COM1);
    serial_println_hex((uint32_t)colour_palette, COM1);

    nov_uvector2 local_position;
    nov_uvector2 global_position = origin;

    uint16_t global_index = (origin.u*buffer_size.v)+origin.v;
    uint16_t mod_index = 0;
    nov_colour colour;
    while (local_position.u <= size.u && local_position.v <= size.v)
    {
        // TODO: find the correct index into the slices
        serial_println_dec(sliced_window[global_index%625], COM1);
        colour = colour_palette[sliced_window[global_index%625]]*255.0f;
        serial_print((char*)"colour: ", COM1); serial_print_dec(colour.x, COM1); serial_print((char*)",", COM1); serial_print_dec(colour.y, COM1); serial_print((char*)",", COM1); serial_println_dec(colour.z, COM1);
        buffer[(global_index*3)+0] = colour.z;
        buffer[(global_index*3)+1] = colour.y;
        buffer[(global_index*3)+2] = colour.x;

        local_position.u++;
        global_position.u++;
        global_index++;
        if (local_position.u > size.u)
        {
            local_position.u = 0;
            local_position.v++;

            global_position.u = origin.u;
            global_position.v++;

            global_index -= size.u+1;
            global_index += buffer_size.u;
        }
        mod_index++;
        mod_index = mod_index%5;
    }
}
