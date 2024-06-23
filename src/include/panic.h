#pragma once

#include <serial.h>
#include <assembly.h>

namespace nov
{

inline void panic()
{
    serial::com_1 << "PANIC" << stream::endl;
    serial::com_1 << "EIP : " << stream::Mode::HEX << getEIP() << stream::endl;
    serial::com_1.flush();

    while (true) hlt();
}

inline void panic(const char* message)
{
    serial::com_1 << "PANIC" << stream::endl;
    serial::com_1 << "EIP : " << stream::Mode::HEX << getEIP() << stream::endl;
    serial::com_1 << "MSG : " << message << stream::endl;
    serial::com_1.flush();

    while (true) hlt();
}

}