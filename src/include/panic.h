#pragma once

#include <serial.h>
#include <assembly.h>

namespace nov
{

inline void panic()
{
    serial_println((char*)"PANIC", COM1);
    serial_print((char*)"EIP : ", COM1);  serial_println_hex(get_eip(), COM1);

    while (true) hlt();
}

}