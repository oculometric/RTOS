#pragma once

#include <serial.h>
#include <assembly.h>

namespace nov
{

inline void panic()
{
    com_1 << "PANIC" << stream::endl;
    com_1 << "EIP : " << stream::mode::HEX << get_eip() << stream::endl;
    com_1.flush();

    while (true) hlt();
}

}