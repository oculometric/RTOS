#include <keyboard.h>

#include <io.h>
#include <serial.h>
#include <scancodes.h>

namespace nov
{
namespace keyboard
{

PS2KeyboardController* keyboard_controller = 0x0;

void keyboardInterruptCallback()
{
    if (keyboard_controller) keyboard_controller->queueInByte(inb(PS2_DATA_PORT));
    else inb(PS2_DATA_PORT);
}

void assignPS2KeyboardController(PS2KeyboardController* controller)
{
    keyboard_controller = controller;
}

KeyState decodeScancode(uint8_t scancode, ScancodeSet scs)
{
    KeyState state;
    switch (scs)
    {
    case ScancodeSet::SET_1:
        state.key = scan_code_set_1_char[scancode];
        state.is_down = scan_code_set_1_down[scancode];
        break;
    default:
        state.key = 0;
        state.is_down = 0;
        break;
    }
    return state;
}

void PS2KeyboardController::queueInByte(uint8_t byte)
{
    if (in_queue_end == in_queue_start - 1)
        in_queue_start++;
    in_queue[in_queue_end++] = byte;
}

PS2KeyboardController::PS2KeyboardController()
{
}

uint8_t PS2KeyboardController::dequeueInByte()
{
    if (in_queue_start == in_queue_end) return 0x0;
    return in_queue[in_queue_start++];
}

}
}