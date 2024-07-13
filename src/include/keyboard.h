#pragma once

#include <stdint.h>

namespace nov
{
namespace keyboard
{

enum Key
{
    K_NONE          = 0x00,
    K_L_GUI,
    K_R_GUI,
    K_L_CTRL,
    K_R_CTRL,
    K_L_ALT,
    K_R_ALT,
    K_APPS,
    K_BACKSPACE,
    K_TAB,
    K_ENTER,
    K_CAPSLOCK,
    K_NUMLOCK,
    K_SCROLLLOCK,
    K_L_SHIFT,
    K_R_SHIFT,
    K_N_0           = 0x10,
    K_N_1,
    K_N_2,
    K_N_3,
    K_N_4,
    K_N_5,
    K_N_6,
    K_N_7,
    K_N_8,
    K_N_9,
    K_BACKSLASH,
    K_ESCAPE,
    K_BACKTICK,
    K_DASH,
    K_EQUALS,

    K_SPACEBAR      = 0x20,
    K_OSQBRACKET,
    K_CSQBRACKET,
    K_SEMICOLON,
    K_QUOTE,
    K_DOT,
    K_COMMA,
    K_FORWSLASH,
    K_N_DOT,
    K_N_SLASH,
    K_N_STAR,
    K_N_DASH,
    K_N_PLUS,
    K_N_ENTER,
    K_INSERT,
    K_DELETE,
    K_0             = 0x30,
    K_1,
    K_2,
    K_3,
    K_4,
    K_5,
    K_6,
    K_7,
    K_8,
    K_9,
    K_HOME,
    K_END,
    K_PAGEUP,
    K_PAGEDOWN,
    K_PRINTSCREEN,
    K_PAUSE,
    
    K_A             = 0x41,
    K_B,
    K_C,
    K_D,
    K_E,
    K_F,
    K_G,
    K_H,
    K_I,
    K_J,
    K_K,
    K_L,
    K_M,
    K_N,
    K_O,
    K_P             = 0x50,
    K_Q,
    K_R,
    K_S,
    K_T,
    K_U,
    K_V,
    K_W,
    K_X,
    K_Y,
    K_Z,
    K_POWER,
    K_SLEEP,
    K_WAKE,

    
    K_F_1           = 0x60,
    K_F_2,
    K_F_3,
    K_F_4,
    K_F_5,
    K_F_6,
    K_F_7,
    K_F_8,
    K_F_9,
    K_F_10,
    K_F_11,
    K_F_12,
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_M_NEXT        = 0x70,
    K_M_PREV,
    K_M_STOP,
    K_M_PAUSE,
    K_M_MUTE,
    K_M_VOLUP,
    K_M_VOLDOWN,
    K_M_SELECT,
    K_M_EMAIL,
    K_M_CALC,
    K_M_COMPUTER,
    K_W_SEARCH,
    K_W_HOME,
    K_W_BACK,
    K_W_FORWARD,
    K_W_STOP,
    K_W_REFRESH     = 0x80,
    K_W_FAVOURITES
};

const char* toString(Key key);
uint8_t getAscii(Key key, bool capitalised, bool shifted, bool numlocked);

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

enum Modifier
{
    L_SHIFT   = 0b00000001,
    L_CTRL    = 0b00000010,
    L_GUI     = 0b00000100,
    L_ALT     = 0b00001000,
    R_SHIFT   = 0b00010000,
    R_CTRL    = 0b00100000,
    R_GUI     = 0b01000000,
    R_ALT     = 0b10000000
};

struct KeyEvent
{
    bool is_down;
    Key key;
    uint8_t ascii;
    uint8_t modifiers;
};

enum ScancodeSet
{
    SET_1,
    SET_2,
    SET_3
};

// TODO: remove/convert this
// TODO: implement commands to control the keyboard
// TODO: add scancodeset 2
class PS2KeyboardController
{
private:
    KeyboardCommand command_buffer[256];

public:
    void queueInByte(uint8_t byte);

    PS2KeyboardController(PS2KeyboardController& other) = delete;
    PS2KeyboardController(PS2KeyboardController&& other) = delete;

    PS2KeyboardController();
};

void keyboardInterruptCallback();

#define PS2_DATA_PORT 0x60

class KeyboardDriver
{
private:
    uint64_t key_states_low = 0;
    uint64_t key_states_high = 0;

    bool caps_lock = false;
    bool num_lock = false;
    bool scroll_lock = false;

    KeyEvent event_queue[256];
    uint8_t event_queue_start = 0;
    uint8_t event_queue_end = 0;

    void enqueueEvent(KeyEvent event);
    KeyEvent dequeueEvent();

    void setKeyState(Key key, bool down);

    uint8_t scancode_buffer[4];
    uint8_t scancode_buffer_index = 0;

    void processScancodeBuffer();

public:
    void receiveScanCode(uint8_t value);

    bool getKeyState(Key key);
    bool hasEventWaiting();
    KeyEvent pollNextEvent();

    inline KeyboardDriver() { }

    KeyboardDriver(KeyboardDriver& other) = delete;
    KeyboardDriver(KeyboardDriver&& other) = delete;
    KeyboardDriver operator=(const KeyboardDriver& other) = delete;
    KeyboardDriver operator=(KeyboardDriver&& other) = delete;
};

void assignKeyboardDriver(KeyboardDriver* controller);

bool keyboardGetKeyState(Key key);
bool keyboardHasEventWaiting();
KeyEvent keyboardPollNextEvent();

}
}