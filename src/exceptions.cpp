#include <exceptions.h>

#include <serial.h>
#include <interrupts.h>

namespace nov
{
namespace exception
{

void handleDivisionError()
{
    com_1 << "--> oh shit! fatal exception: #DE - division error" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleBoundRangeExceeded()
{
    com_1 << "--> oh shit! fatal exception: #BR - bound range exceeded" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleInvalidOpcode()
{
    com_1 << "--> oh shit! fatal exception: #UD - invalid opcode (doesn't rhyme does it)" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleDeviceNotAvailable()
{
    com_1 << "--> oh shit! fatal exception: #NM - device not available (doesn't rhyme does it)" << stream::endl;
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

void handleSegmentNotPresent()
{
    com_1 << "--> oh shit! fatal exception: #NP - segment not present" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleStackSegmentFault()
{
    com_1 << "--> oh shit! fatal exception: #SS - stack segment fault" << stream::endl;
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

void handlePageFault()
{
    com_1 << "--> oh shit! fatal exception: #PF - page fault" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handlex87Exception()
{
    com_1 << "--> oh shit! fatal exception: #MF - x87 floating point exception" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleAlignmentCheck()
{
    com_1 << "--> oh shit! fatal exception: #AC - alignment check" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleMachineCheck()
{
    com_1 << "--> oh shit! fatal exception: #MC - machine check" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleSIMDException()
{
    com_1 << "--> oh shit! fatal exception: #XM/#XF - SIMD floating point exception" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleVirtualisationException()
{
    com_1 << "--> oh shit! fatal exception: #VE - virtualisation exception" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleControlProtectionException()
{
    com_1 << "--> oh shit! fatal exception: #CP - control protection exception" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleHypervisorInjectionException()
{
    com_1 << "--> oh shit! fatal exception: #HV - hypervisor injection exception" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleVMMCommunicationException()
{
    com_1 << "--> oh shit! fatal exception: #VC - VMM communication exception" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}

void handleSecurityException()
{
    com_1 << "--> oh shit! fatal exception: #SX - security exception" << stream::endl;
    com_1 << "halting." << stream::endl;
    com_1.flush();
    while (true) { }
}


void registerExceptionHandlers()
{
    interrupts::configureInterruptHandler
    (
        0x00, 
        handleDivisionError, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x05, 
        handleBoundRangeExceeded, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x06, 
        handleInvalidOpcode, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x07, 
        handleDeviceNotAvailable, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x08, 
        handleDoubleFault, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x0a, 
        handleInvalidTSS, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x0b, 
        handleSegmentNotPresent, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );
    
    interrupts::configureInterruptHandler
    (
        0x0c, 
        handleStackSegmentFault, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x0d, 
        handleGeneralProtectionFault, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );
        
    interrupts::configureInterruptHandler
    (
        0x10, 
        handlex87Exception, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x11, 
        handleAlignmentCheck, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x12, 
        handleMachineCheck, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x13, 
        handleSIMDException, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x14, 
        handleVirtualisationException, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x15, 
        handleControlProtectionException, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x1C, 
        handleHypervisorInjectionException, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x1D, 
        handleVMMCommunicationException, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );

    interrupts::configureInterruptHandler
    (
        0x1E, 
        handleSecurityException, 
        interrupts::GateType::TRAP_32, 
        Privilege::LEVEL_0
    );
}

}
}