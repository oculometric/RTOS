#pragma once

namespace nov
{
namespace pit
{

enum Port
{
    CHANNEL_0 = 0x40,
    CHANNEL_1 = 0x41,
    CHANNEL_2 = 0x42,
    COMMAND   = 0x43
};

enum CommandRegChannel
{
    PIT_CHANNEL_0 = 0b00,
    PIT_CHANNEL_1 = 0b01,
    PIT_CHANNEL_2 = 0b10,
    READBACK  = 0b11
};

enum CommandRegAccessMode
{
    LATCH_COUNT_VALUE = 0b00,
    ACCESS_LOBYTE     = 0b01,
    ACCESS_HIBYTE     = 0b10,
    ACCESS_BOTH       = 0b11
};

enum CommandRegOperatingMode
{
    INTERRUPT_ON_TERMINAL_COUNT,
    HARDWARE_RETRIGGERABLE_ONE_SHOT,
    RATE_GENERATOR,
    SQUARE_WAVE_GENERATOR,
    SOFTWARE_TRIGGERED_STROBE,
    HARDWARE_TRIGGERED_STROBE
};


void timerInterruptCallback();
// TODO: timer configuration
// TODO: sleep function

}
}