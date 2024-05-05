#include <exceptions.h>

#include <serial.h>
#include <interrupts.h>

namespace nov
{
namespace exception
{

void handlePageFault()
{
    com_1 << "--> oh shit! fatal exception: #PF - page fault" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleGeneralProtectionFault()
{
    com_1 << "--> oh shit! fatal exception: #GP - general protection" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleDoubleFault()
{
    com_1 << "--> oh shit! fatal exception: #DF - double fault" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleInvalidTSS()
{
    com_1 << "--> oh shit! fatal exception: #TS - invalid task segment" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void registerExceptionHandlers()
{
    interrupts::configureInterruptHandler
    (
        0x08, 
        handleDoubleFault, 
        interrupts::GateType::TRAP_32, 
        interrupts::Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x0a, 
        handleInvalidTSS, 
        interrupts::GateType::TRAP_32, 
        interrupts::Privilege::LEVEL_0
    );
    
    interrupts::configureInterruptHandler
    (
        0x0d, 
        handleGeneralProtectionFault, 
        interrupts::GateType::TRAP_32, 
        interrupts::Privilege::LEVEL_0
    );
        
    interrupts::configureInterruptHandler
    (
        0x0e, 
        handlePageFault, 
        interrupts::GateType::TRAP_32, 
        interrupts::Privilege::LEVEL_0
    );
}

}
}