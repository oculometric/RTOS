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

namespace nov
{
namespace keyboard
{

enum KeyboardCommandByte
{
    SET_LED_STATE = 0xED,
    ECHO = 0xEE,
    SCAN_CODE_SET = 0xF0,
    IDENTIFY = 0xF2,
    SET_TYPEMATIC = 0xF3,
    ENABLE_SCANNING = 0xF4,
    DISABLE_SCANNING = 0xF5,
    SET_DEFAULTS = 0xF6,
    SET_ALL_TYPEMATIC_ONLY = 0xF7,
    SET_ALL_MR_ONLY = 0xF8,
    SET_ALL_MAKE_ONLY = 0xF9,
    SET_ALL_TMR_ONLY = 0xFA,
    SET_TYPEMATIC_ONLY = 0xFB,
    SET_MR_ONLY = 0xFC,
    SET_MAKE_ONLY = 0xFD,
    RESEND = 0xFE,
    RESET = 0xFF
};

struct KeyboardCommand
{

};

struct KeyState
{
    bool is_down;
    char key;
};

enum ScancodeSet
{
    SET_1,
    SET_2,
    SET_3
};

class PS2KeyboardController
{
private:
    uint8_t in_queue[256];
    uint8_t in_queue_start = 0;
    uint8_t in_queue_end = 0;

    KeyboardCommand command_buffer[256];

public:
    uint8_t dequeueInByte();
    void queueInByte(uint8_t byte);

    PS2KeyboardController(PS2KeyboardController& other) = delete;
    PS2KeyboardController(PS2KeyboardController&& other) = delete;

    PS2KeyboardController();
};

void keyboardInterruptCallback();
void assignPS2KeyboardController(PS2KeyboardController* controller);

KeyState decodeScancode(uint8_t scancode, ScancodeSet scs);
bool isAlphaNumeric(char c);

#define PS2_DATA_PORT 0x60

}
}