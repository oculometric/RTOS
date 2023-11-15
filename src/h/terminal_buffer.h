#pragma once

#include <stdint.h>
#include <stddef.h>

#include "vector2.h"
#include "memory.h"

enum layer
{
    BACKGROUND,
    FOREGROUND,
    OVERLAY
};

class terminal_buffer
{
private:
    uint8_t* background_buffer_t;
    uint8_t* background_buffer_c;
    uint8_t* foreground_buffer_t;
    uint8_t* foreground_buffer_c;
    uint8_t* overlay_buffer_t;
    uint8_t* overlay_buffer_c;

    vector2<unsigned int> buffer_size;
    size_t buffer_length;
    
    // b = true for colour, b = false for text/char value
    inline uint8_t* buffer_ptr(layer l, bool b)
    {
        switch (l)
        {
        case BACKGROUND: return b ? background_buffer_c : background_buffer_t;
        case FOREGROUND: return b ? foreground_buffer_c : foreground_buffer_t;
        case OVERLAY: return b ? overlay_buffer_c : overlay_buffer_t;
        }
    }

public:
    inline vector2<unsigned int> get_size() { return buffer_size; }
    inline unsigned int get_length() { return buffer_length; }

    inline void set_tile_t(layer l, vector2<unsigned int> t, uint8_t v) { set_tile_t(l, t.y*buffer_size.x + t.x, v); }
    inline void set_tile_t(layer l, size_t o, uint8_t v) { buffer_ptr(l, 0)[o] = v; }

    inline void set_tile_c(layer l, vector2<unsigned int> t, uint8_t c) { set_tile_c(l, t.y*buffer_size.x + t.x, c); }
    inline void set_tile_c(layer l, size_t o, uint8_t c) { buffer_ptr(l, 1)[o] = c; }

    inline uint8_t get_tile_t(layer l, vector2<unsigned int> t) { return get_tile_t(l, t.y*buffer_size.x + t.x); }
    inline uint8_t get_tile_t(layer l, size_t o) { return buffer_ptr(l, 0)[o]; }

    inline uint8_t get_tile_c(layer l, vector2<unsigned int> t) { return get_tile_c(l, t.y*buffer_size.x + t.x); }
    inline uint8_t get_tile_c(layer l, size_t o) { return buffer_ptr(l, 1)[o]; }

    inline void fill_layer_t(layer l, uint8_t t) { uint8_t* ptr = buffer_ptr(l, 0); for (size_t i = 0; i < buffer_length; i++) ptr[i] = t; }
    inline void fill_layer_c(layer l, uint8_t c) { uint8_t* ptr = buffer_ptr(l, 1); for (size_t i = 0; i < buffer_length; i++) ptr[i] = c; }

    inline terminal_buffer(vector2<unsigned int> size)
    {
        buffer_size = size;
        buffer_length = size.x*size.y;

        background_buffer_t = malloc(buffer_length);
        background_buffer_c = malloc(buffer_length);

        foreground_buffer_t = malloc(buffer_length);
        foreground_buffer_c = malloc(buffer_length);
        
        overlay_buffer_t = malloc(buffer_length);
        overlay_buffer_c = malloc(buffer_length);
    }

    inline ~terminal_buffer(); // TODO

    void draw(uint8_t*);
};