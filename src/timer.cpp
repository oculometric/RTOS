#include <timer.h>

#include <serial.h>

namespace nov
{
namespace timer
{

void timerInterruptCallback()
{
    serial::com_1 << "timer" << stream::endl;
}
    
}
}