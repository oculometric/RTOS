#pragma once

#include <stdint.h>

#define K_ESC '\x1b'
#define K_LSHIFT '\x0e'
#define K_RSHIFT '\x0f'
#define K_LCTRL '\x11'
#define K_RCTRL '\x12'
#define K_LALT '\x13'
#define K_NSTAR '\x01'
#define K_NPLUS '\x02'
#define K_NMINUS '\x03'
#define K_NDOT '\x04'
#define K_CAPS '\x05'
#define K_SCROLL '\x06'
#define K_NUM '\x07'
#define K_N0 '\xb0'
#define K_N1 '\xb1'
#define K_N2 '\xb2'
#define K_N3 '\xb3'
#define K_N4 '\xb4'
#define K_N5 '\xb5'
#define K_N6 '\xb6'
#define K_N7 '\xb7'
#define K_N8 '\xb8'
#define K_N9 '\xb9'
#define K_F1 '\xba'
#define K_F2 '\xbb'
#define K_F3 '\xbc'
#define K_F4 '\xbd'
#define K_F5 '\xbe'
#define K_F6 '\xbf'
#define K_F7 '\xc0'
#define K_F8 '\xc1'
#define K_F9 '\xc2'
#define K_F10 '\xc3'
#define K_F11 '\xc4'
#define K_F12 '\xc5'

static const char scan_code_set_1_char[256]
{
    '\0',   K_ESC,   '1',      '2',     '3',   '4',     '5',      '6',
    '7',    '8',     '9',      '0',     '-',   '=',     '\b',     '\t',
    'q',    'w',     'e',      'r',     't',   'y',     'u',      'i',
    'o',    'p',     '[',      ']',     '\n',  K_LCTRL, 'a',      's',
    'd',    'f',     'g',      'h',     'j',   'k',     'l',      ';',
    '\'',   '`',     K_LSHIFT, '\\',    'z',   'x',     'c',      'v',
    'b',    'n',     'm',      ',',     '.',   '/',     K_RSHIFT, K_NSTAR,
    K_LALT, ' ',     K_CAPS,   K_F1,    K_F2,  K_F3,    K_F4,     K_F5,
    K_F6,   K_F7,    K_F8,     K_F9,    K_F10, K_NUM,   K_SCROLL, K_N7,
    K_N8,   K_N9,    K_NMINUS, K_N4,    K_N5,  K_N6,    K_NPLUS,  K_N1,
    K_N2,   K_N3,    K_N0,     K_NDOT,  '\0',  '\0',    '\0',     K_F11,
    K_F12,  '\0',    '\0',     '\0',    '\0',  K_ESC,   '1',      '2',
    '3',    '4',     '5',      '6',     '7',   '8',     '9',      '0',
    '-',    '=',     '\b',     '\t',    'q',   'w',     'e',      'r',
    't',    'y',     'u',      'i',     'o',   'p',     '[',      ']',
    '\n',   K_LCTRL, 'a',      's',     'd',   'f',     'g',      'h',
    'j',    'k',     'l',      ';',     '\'',  '`',     K_LSHIFT, '\\',
    'z',    'x',     'c',      'v',     'b',   'n',     'm',      ',',
    '.',    '/',     K_RSHIFT, K_NSTAR, K_LALT, ' ',    K_CAPS,   K_F1,
    K_F2,   K_F3,    K_F4,     K_F5,    K_F6,  K_F7,    K_F8,     K_F9,
    K_F10,  K_NUM,   K_SCROLL, K_N7,    K_N8,  K_N9,    K_NMINUS, K_N4,
    K_N5,   K_N6,    K_NPLUS,  K_N1,    K_N2,  K_N3,    K_N0,     K_NDOT,
    '\0',   '\0',    '\0',     K_F11,   K_F12, '\0',    '\0',     '\0'
};

static const bool scan_code_set_1_down[256]
{
    0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};