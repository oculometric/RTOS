#pragma once

#include <stdint.h>

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

#define PS2_DATA_PORT 0x60

}
}