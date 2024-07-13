#include <pit.h>

#include <serial.h>

namespace nov
{
namespace pit
{

void timerInterruptCallback()
{
    serial::com_1 << "timer" << stream::endl;
}
    
}
}