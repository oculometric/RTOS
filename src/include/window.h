#pragma once

#include <stdint.h>

#include <vector.h>

#define WINDOW_SLICE_SIZE 5
#define WINDOW_SLICE_AREA WINDOW_SLICE_SIZE*WINDOW_SLICE_SIZE
#define WINDOW_SLICE_NUM 25

#define WINDOW_TOPLEFTCORNER 0
#define WINDOW_TOPEDGE 1
#define WINDOW_TOPJUNCTION 2
#define WINDOW_TOPRIGHTCORNER 3
#define WINDOW_LEFTJUNCTION 4
#define WINDOW_HORIZONTALEDGE 5
#define WINDOW_XJUNCTION 6
#define WINDOW_RIGHTJUNCTION 7
#define WINDOW_LEFTEDGE 8
#define WINDOW_SOLID 9
#define WINDOW_VERTICALEDGE 10
#define WINDOW_RIGHTEDGE 11
#define WINDOW_BOTTOMLEFTCORNER 12
#define WINDOW_BOTTOMEDGE 13
#define WINDOW_BOTTOMJUNCTION 14
#define WINDOW_BOTTOMRIGHTCORNER 15
#define WINDOW_SYMBOL_0 16
#define WINDOW_SYMBOL_1 17
#define WINDOW_SYMBOL_2 18
#define WINDOW_SYMBOL_3 19
#define WINDOW_SYMBOL_4 20
#define WINDOW_SYMBOL_5 21
#define WINDOW_SYMBOL_6 22
#define WINDOW_SYMBOL_7 23
#define WINDOW_SYMBOL_8 24

const uint8_t sliced_window[WINDOW_SLICE_SIZE*WINDOW_SLICE_SIZE*WINDOW_SLICE_NUM] =
{
    // top left corner
    0, 1, 1, 1, 1,
    3, 2, 2, 2, 2,
    3, 2, 2, 3, 3,
    3, 2, 1, 2, 4,
    3, 2, 1, 4, 2,
    // top edge
    1, 1, 1, 1, 1,
    2, 2, 2, 2, 2,
    3, 3, 3, 3, 3,
    4, 4, 4, 4, 4,
    1, 1, 1, 1, 1,
    // top junction
    1, 1, 1, 1, 1,
    2, 2, 2, 2, 2,
    3, 3, 3, 3, 3,
    4, 4, 4, 4, 4,
    1, 1, 4, 2, 1,
    // top right corner
    1, 1, 1, 1, 0,
    2, 2, 2, 1, 1,
    3, 3, 2, 2, 1,
    4, 3, 3, 2, 1,
    1, 4, 3, 2, 1,
    // left junction
    3, 2, 1, 4, 3,
    3, 2, 1, 4, 3,
    3, 2, 1, 4, 4,
    3, 2, 1, 4, 2,
    3, 2, 1, 4, 3,
    // horizontal edge
    2, 2, 2, 2, 2,
    3, 3, 3, 3, 3,
    4, 4, 4, 4, 4,
    1, 1, 1, 1, 1,
    2, 2, 2, 2, 2,
    // x junction
    2, 1, 4, 3, 2,
    3, 2, 4, 3, 3,
    4, 4, 4, 4, 4,
    1, 1, 4, 2, 1,
    2, 1, 4, 3, 2,
    // right junction
    1, 4, 3, 2, 1,
    2, 4, 3, 2, 1,
    4, 4, 3, 2, 1,
    1, 4, 3, 2, 1,
    1, 4, 3, 2, 1,
    // left edge
    3, 2, 1, 4, 3,
    3, 2, 1, 4, 3,
    3, 2, 1, 4, 3,
    3, 2, 1, 4, 3,
    3, 2, 1, 4, 3,
    // solid
    2, 2, 2, 2, 2,
    2, 2, 2, 2, 2,
    2, 2, 2, 2, 2,
    2, 2, 2, 2, 2,
    2, 2, 2, 2, 2,
    // vertical edge
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    // right edge
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    2, 1, 4, 3, 2,
    // bottom left corner
    3, 2, 1, 4, 3,
    3, 2, 1, 1, 4,
    3, 2, 2, 1, 1,
    4, 3, 2, 2, 2,
    0, 4, 3, 3, 3,
    // bottom edge
    3, 3, 3, 3, 3,
    4, 4, 4, 4, 4,
    1, 1, 1, 1, 1,
    2, 2, 2, 2, 2,
    3, 3, 3, 3, 3,
    // bottom junction
    3, 2, 4, 3, 3,
    4, 4, 4, 4, 4,
    1, 1, 1, 1, 1,
    2, 2, 2, 2, 2,
    3, 3, 3, 3, 3,
    // bottom right corner
    2, 4, 3, 2, 1,
    4, 2, 3, 2, 1,
    1, 1, 2, 2, 1,
    2, 2, 2, 2, 1,
    3, 3, 3, 3, 0,
    // r symbol 0
    2, 2, 2, 2, 2,
    1, 3, 3, 3, 1,
    1, 3, 2, 3, 2,
    1, 3, 2, 1, 2,
    1, 1, 2, 2, 2,
    // = symbol 1
    2, 2, 2, 2, 2,
    1, 3, 3, 3, 2,
    2, 2, 3, 2, 2,
    1, 3, 3, 3, 2,
    2, 1, 1, 1, 2,
    // T symbol 2
    2, 2, 2, 2, 2,
    2, 2, 2, 3, 2,
    1, 3, 3, 3, 2,
    1, 1, 1, 3, 2,
    2, 2, 2, 1, 2,
    // K symbol 3
    2, 2, 2, 2, 2,
    1, 3, 2, 3, 2,
    2, 1, 3, 3, 2,
    2, 2, 1, 3, 2,
    2, 2, 2, 1, 2,
    // D symbol 4
    2, 2, 2, 2, 2,
    1, 3, 3, 3, 2,
    2, 2, 2, 3, 2,
    1, 3, 3, 2, 2,
    1, 1, 1, 2, 2,
    // H symbol 5
    2, 2, 2, 2, 2,
    1, 3, 2, 3, 2,
    1, 3, 1, 3, 2,
    1, 3, 1, 3, 2,
    1, 1, 2, 1, 2,
    // X symbol 6
    2, 2, 2, 2, 2,
    1, 3, 2, 3, 2,
    2, 1, 3, 2, 2,
    1, 3, 1, 3, 2,
    2, 1, 2, 1, 2,
    // - symbol 7
    2, 2, 2, 2, 2,
    2, 2, 2, 2, 2,
    1, 3, 3, 3, 2,
    1, 1, 1, 1, 2,
    2, 2, 2, 2, 2,
    // O symbol 8
    2, 2, 2, 2, 2,
    1, 3, 3, 3, 2,
    1, 3, 2, 3, 2,
    1, 3, 3, 3, 2,
    1, 1, 1, 1, 2
};

static const nov_uvector3 colour_palette[5] =
{
    nov_uvector3{ 0,0,0 },
    nov_uvector3{ 204,204,204 },
    nov_uvector3{ 128,128,128 },
    nov_uvector3{ 26,26,26 },
    nov_uvector3{ 3,3,3 }
};

void draw_window(const nov_uvector2& origin, const nov_uvector2& size, uint8_t* buffer, const nov_uvector2& buffer_size);