#include <keyboard.h>

#include <io.h>
#include <serial.h>
#include <scancodes.h>

namespace nov
{
namespace keyboard
{

KeyboardDriver* keyboard_driver = 0x0;

const char* toString(Key key)
{
    switch (key)
    {
    case K_NONE: return "K_NONE";
    case K_L_GUI: return "K_L_GUI";
    case K_R_GUI: return "K_R_GUI";
    case K_L_CTRL: return "K_L_CTRL";
    case K_R_CTRL: return "K_R_CTRL";
    case K_L_ALT: return "K_L_ALT";
    case K_R_ALT: return "K_R_ALT";
    case K_APPS: return "K_APPS";
    case K_BACKSPACE: return "K_BACKSPACE";
    case K_TAB: return "K_TAB";
    case K_ENTER: return "K_ENTER";
    case K_CAPSLOCK: return "K_CAPSLOCK";
    case K_NUMLOCK: return "K_NUMLOCK";
    case K_SCROLLLOCK: return "K_SCROLLLOCK";
    case K_L_SHIFT: return "K_L_SHIFT";
    case K_R_SHIFT: return "K_R_SHIFT";
    case K_N_0: return "K_N_0";
    case K_N_1: return "K_N_1";
    case K_N_2: return "K_N_2";
    case K_N_3: return "K_N_3";
    case K_N_4: return "K_N_4";
    case K_N_5: return "K_N_5";
    case K_N_6: return "K_N_6";
    case K_N_7: return "K_N_7";
    case K_N_8: return "K_N_8";
    case K_N_9: return "K_N_9";
    case K_BACKSLASH: return "K_BACKSLASH";
    case K_ESCAPE: return "K_ESCAPE";
    case K_BACKTICK: return "K_BACKTICK";
    case K_DASH: return "K_DASH";
    case K_EQUALS: return "K_EQUALS";
    case K_SPACEBAR: return "K_SPACEBAR";
    case K_OSQBRACKET: return "K_OSQBRACKET";
    case K_CSQBRACKET: return "K_CSQBRACKET";
    case K_SEMICOLON: return "K_SEMICOLON";
    case K_QUOTE: return "K_QUOTE";
    case K_DOT: return "K_DOT";
    case K_COMMA: return "K_COMMA";
    case K_FORWSLASH: return "K_FORWSLASH";
    case K_N_DOT: return "K_N_DOT";
    case K_N_SLASH: return "K_N_SLASH";
    case K_N_STAR: return "K_N_STAR";
    case K_N_DASH: return "K_N_DASH";
    case K_N_PLUS: return "K_N_PLUS";
    case K_N_ENTER: return "K_N_ENTER";
    case K_INSERT: return "K_INSERT";
    case K_DELETE: return "K_DELETE";
    case K_0: return "K_0";
    case K_1: return "K_1";
    case K_2: return "K_2";
    case K_3: return "K_3";
    case K_4: return "K_4";
    case K_5: return "K_5";
    case K_6: return "K_6";
    case K_7: return "K_7";
    case K_8: return "K_8";
    case K_9: return "K_9";
    case K_HOME: return "K_HOME";
    case K_END: return "K_END";
    case K_PAGEUP: return "K_PAGEUP";
    case K_PAGEDOWN: return "K_PAGEDOWN";
    case K_PRINTSCREEN: return "K_PRINTSCREEN";
    case K_PAUSE: return "K_PAUSE";
    case K_A: return "K_A";
    case K_B: return "K_B";
    case K_C: return "K_C";
    case K_D: return "K_D";
    case K_E: return "K_E";
    case K_F: return "K_F";
    case K_G: return "K_G";
    case K_H: return "K_H";
    case K_I: return "K_I";
    case K_J: return "K_J";
    case K_K: return "K_K";
    case K_L: return "K_L";
    case K_M: return "K_M";
    case K_N: return "K_N";
    case K_O: return "K_O";
    case K_P: return "K_P";
    case K_Q: return "K_Q";
    case K_R: return "K_R";
    case K_S: return "K_S";
    case K_T: return "K_T";
    case K_U: return "K_U";
    case K_V: return "K_V";
    case K_W: return "K_W";
    case K_X: return "K_X";
    case K_Y: return "K_Y";
    case K_Z: return "K_Z";
    case K_POWER: return "K_POWER";
    case K_SLEEP: return "K_SLEEP";
    case K_WAKE: return "K_WAKE";
    case K_F_1: return "K_F_1";
    case K_F_2: return "K_F_2";
    case K_F_3: return "K_F_3";
    case K_F_4: return "K_F_4";
    case K_F_5: return "K_F_5";
    case K_F_6: return "K_F_6";
    case K_F_7: return "K_F_7";
    case K_F_8: return "K_F_8";
    case K_F_9: return "K_F_9";
    case K_F_10: return "K_F_10";
    case K_F_11: return "K_F_11";
    case K_F_12: return "K_F_12";
    case K_UP: return "K_UP";
    case K_DOWN: return "K_DOWN";
    case K_LEFT: return "K_LEFT";
    case K_RIGHT: return "K_RIGHT";
    case K_M_NEXT: return "K_M_NEXT";
    case K_M_PREV: return "K_M_PREV";
    case K_M_STOP: return "K_M_STOP";
    case K_M_PAUSE: return "K_M_PAUSE";
    case K_M_MUTE: return "K_M_MUTE";
    case K_M_VOLUP: return "K_M_VOLUP";
    case K_M_VOLDOWN: return "K_M_VOLDOWN";
    case K_M_SELECT: return "K_M_SELECT";
    case K_M_EMAIL: return "K_M_EMAIL";
    case K_M_CALC: return "K_M_CALC";
    case K_M_COMPUTER: return "K_M_COMPUTER";
    case K_W_SEARCH: return "K_W_SEARCH";
    case K_W_HOME: return "K_W_HOME";
    case K_W_BACK: return "K_W_BACK";
    case K_W_FORWARD: return "K_W_FORWARD";
    case K_W_STOP: return "K_W_STOP";
    case K_W_REFRESH: return "K_W_REFRESH";
    case K_W_FAVOURITES: return "K_W_FAVOURITES";
    }
    return "K_UNKNOWN";
}

static const uint8_t num_specials[10] = { ')', '!', '@', '#', '$', '%', '^', '&', '*', '(' };

uint8_t getAscii(Key key, bool capitalised, bool shifted, bool numlocked)
{
    if (key == Key::K_BACKSPACE) return '\b';
    else if (key == Key::K_TAB) return '\t';
    else if (key == Key::K_ENTER) return '\n';
    else if (key >= Key::K_N_0 && key <= Key::K_N_9 && numlocked) return (key - Key::K_N_0) + Key::K_0;
    else if (key == Key::K_BACKSLASH) return shifted ? '|' : '\\';
    else if (key == Key::K_BACKTICK) return shifted ? '~' : '`';
    else if (key == Key::K_DASH) return shifted ? '_' : '-';
    else if (key == Key::K_EQUALS) return shifted ? '+' : '=';
    else if (key == Key::K_SPACEBAR) return ' ';
    else if (key == Key::K_OSQBRACKET) return shifted ? '{' : '[';
    else if (key == Key::K_CSQBRACKET) return shifted ? '}' : ']';
    else if (key == Key::K_SEMICOLON) return shifted ? ':' : ';';
    else if (key == Key::K_QUOTE) return shifted ? '"' : '\'';
    else if (key == Key::K_DOT) return shifted ? '>' : '.';
    else if (key == Key::K_COMMA) return shifted ? '<' : ',';
    else if (key == Key::K_FORWSLASH) return shifted ? '?' : '/';
    else if (key == Key::K_N_DOT) return numlocked ? '.' : 0;
    else if (key == Key::K_N_SLASH) return '/';
    else if (key == Key::K_N_STAR) return '*';
    else if (key == Key::K_N_DASH) return '-';
    else if (key == Key::K_N_PLUS) return '+';
    else if (key == Key::K_N_ENTER) return '\n';
    else if (key >= Key::K_0 && key <= Key::K_9) return shifted ? num_specials[key - Key::K_0] : (uint8_t)key;
    else if (key >= Key::K_A && key <= Key::K_Z) return capitalised ? key : key + 32;
    return 0;
}

void keyboardInterruptCallback()
{
    if (keyboard_driver) keyboard_driver->receiveScanCode(inb(PS2_DATA_PORT));
    else inb(PS2_DATA_PORT);
}

void assignKeyboardDriver(KeyboardDriver* controller)
{
    keyboard_driver = controller;
}

void KeyboardDriver::enqueueEvent(KeyEvent event)
{
    if (event_queue_end == event_queue_start - 1)
        event_queue_start++;
    event_queue[event_queue_end++] = event;
}

KeyEvent KeyboardDriver::dequeueEvent()
{
    if (event_queue_start == event_queue_end)
        return KeyEvent{ 0, Key::K_NONE, 0, 0 };
    return event_queue[event_queue_start++];
}

void KeyboardDriver::setKeyState(Key key, bool down)
{
    if (key < 64)
        key_states_low = (key_states_low & ~(0b1 << key)) | (down << key);
    else if (key < 128)
        key_states_high = (key_states_high & ~(0b1 << (key - 64))) | (down << (key - 64));
    else return;
}

void KeyboardDriver::processScancodeBuffer()
{
    // if there's nothing in the buffer, return
    if (scancode_buffer_index == 0) return;
    // if the buffer contains a prefix code and nothing else, return
    if (scancode_buffer_index == 1 && (scancode_buffer[0] == 0xe0 || scancode_buffer[0] == 0xe1)) return;

    KeyEvent key_event{ 0, Key::K_NONE, 0, 0 };

    if (scancode_buffer_index == 1)
    { // there is one scancode in the buffer
        key_event.key = scan_code_set_1_key[scancode_buffer[0]];
        key_event.is_down = scan_code_set_1_down[scancode_buffer[0]];
    }
    else if (scancode_buffer_index == 2)
    { // there is a preix code and a scancode in the buffer
        key_event.key = scan_code_set_extended_1_key[scancode_buffer[1]];
        key_event.is_down = scan_code_set_extended_1_down[scancode_buffer[1]];
    }

    setKeyState(key_event.key, key_event.is_down);

    // update state of caps, num, and scroll locks
    if (key_event.is_down)
    {
        if (key_event.key == Key::K_CAPSLOCK) caps_lock = !caps_lock;
        else if (key_event.key == Key::K_NUMLOCK) num_lock = !num_lock;
        else if (key_event.key == Key::K_SCROLLLOCK) scroll_lock = !scroll_lock;
    }

    bool should_capitalise = caps_lock ^ (getKeyState(Key::K_L_SHIFT) || getKeyState(Key::K_R_SHIFT));

    // describe the current state of the modifier keys
    key_event.modifiers |= Modifier::L_SHIFT * getKeyState(Key::K_L_SHIFT);
    key_event.modifiers |= Modifier::R_SHIFT * getKeyState(Key::K_R_SHIFT);
    key_event.modifiers |= Modifier::L_CTRL * getKeyState(Key::K_L_CTRL);
    key_event.modifiers |= Modifier::R_CTRL * getKeyState(Key::K_R_CTRL);
    key_event.modifiers |= Modifier::L_ALT * getKeyState(Key::K_L_ALT);
    key_event.modifiers |= Modifier::R_ALT * getKeyState(Key::K_R_ALT);
    key_event.modifiers |= Modifier::L_GUI * getKeyState(Key::K_L_GUI);
    key_event.modifiers |= Modifier::R_GUI * getKeyState(Key::K_R_GUI);

    key_event.ascii = getAscii(key_event.key, should_capitalise, getKeyState(Key::K_L_SHIFT) || getKeyState(Key::K_R_SHIFT), num_lock);

    // reset buffer
    scancode_buffer_index = 0;

    enqueueEvent(key_event);
}

void KeyboardDriver::receiveScanCode(uint8_t value)
{
    scancode_buffer[scancode_buffer_index] = value;
    scancode_buffer_index++;
    processScancodeBuffer();
}

bool KeyboardDriver::getKeyState(Key key)
{
    if (key < 64)
        return key_states_low & (0b1 << key);
    else if (key < 128)
        return key_states_high & (0b1 << (key - 64));
    else return false;
}

bool KeyboardDriver::hasEventWaiting()
{
    return event_queue_start != event_queue_end;
}

KeyEvent KeyboardDriver::pollNextEvent()
{
    return dequeueEvent();
}

}
}