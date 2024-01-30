#include <window.h>

void draw_window(const nov_uvector2& origin, const nov_uvector2& size, uint8_t* buffer, const nov_uvector2& buffer_size)
{
    nov_uvector2 local_position;
    nov_uvector2 global_position = origin;

    uint16_t global_index = (origin.u*buffer_size.v)+origin.v;
    uint16_t mod_index = 0;
    while (local_position.u <= size.u && local_position.v <= size.v)
    {
        // TODO: find the correct index into the slices
        buffer[(global_index*3)+0] = sliced_window[mod_index];
        buffer[(global_index*3)+1] = sliced_window[mod_index];
        buffer[(global_index*3)+2] = sliced_window[mod_index];

        local_position.u++;
        global_position.u++;
        global_index++;
        if (local_position.u > size.u)
        {
            local_position.u = 0;
            local_position.v++;

            global_position.u = origin.u;
            global_position.v++;

            global_index -= size.u;
            global_index += buffer_size.u;
        }
        mod_index++;
        mod_index = mod_index%5;
    }
}
