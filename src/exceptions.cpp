#include <exceptions.h>

#include <serial.h>
#include <interrupts.h>

namespace nov
{
namespace exception
{

void handleDivisionError()
{
    serial::com_1 << "--> oh shit! fatal exception: #DE - division error" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleBoundRangeExceeded()
{
    serial::com_1 << "--> oh shit! fatal exception: #BR - bound range exceeded" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleInvalidOpcode()
{
    serial::com_1 << "--> oh shit! fatal exception: #UD - invalid opcode (doesn't rhyme does it)" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleDeviceNotAvailable()
{
    serial::com_1 << "--> oh shit! fatal exception: #NM - device not available (doesn't rhyme does it)" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleDoubleFault()
{
    serial::com_1 << "--> oh shit! fatal exception: #DF - double fault" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleInvalidTSS()
{
    serial::com_1 << "--> oh shit! fatal exception: #TS - invalid task segment" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleSegmentNotPresent()
{
    serial::com_1 << "--> oh shit! fatal exception: #NP - segment not present" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleStackSegmentFault()
{
    serial::com_1 << "--> oh shit! fatal exception: #SS - stack segment fault" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleGeneralProtectionFault()
{
    serial::com_1 << "--> oh shit! fatal exception: #GP - general protection" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handlePageFault()
{
    serial::com_1 << "--> oh shit! fatal exception: #PF - page fault" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handlex87Exception()
{
    serial::com_1 << "--> oh shit! fatal exception: #MF - x87 floating point exception" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleAlignmentCheck()
{
    serial::com_1 << "--> oh shit! fatal exception: #AC - alignment check" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleMachineCheck()
{
    serial::com_1 << "--> oh shit! fatal exception: #MC - machine check" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleSIMDException()
{
    serial::com_1 << "--> oh shit! fatal exception: #XM/#XF - SIMD floating point exception" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleVirtualisationException()
{
    serial::com_1 << "--> oh shit! fatal exception: #VE - virtualisation exception" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleControlProtectionException()
{
    serial::com_1 << "--> oh shit! fatal exception: #CP - control protection exception" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleHypervisorInjectionException()
{
    serial::com_1 << "--> oh shit! fatal exception: #HV - hypervisor injection exception" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleVMMCommunicationException()
{
    serial::com_1 << "--> oh shit! fatal exception: #VC - VMM communication exception" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
    while (true) { }
}

void handleSecurityException()
{
    serial::com_1 << "--> oh shit! fatal exception: #SX - security exception" << stream::endl;
    serial::com_1 << "halting." << stream::endl;
    serial::com_1.flush();
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