#pragma once

namespace nov
{
namespace exception
{

// handles interrupt 0x00, aka #DE
void handleDivisionError();

// handles interrupt 0x05, aka #BR
void handleBoundRangeExceeded();

// handles interrupt 0x06, aka #UD
void handleInvalidOpcode();

// handles interrupt 0x07, aka #NM
void handleDeviceNotAvailable();

// handles interrupt 0x08, aka #DF
void handleDoubleFault();

// handles interrupt 0x0a, aka #TS
void handleInvalidTSS();

// handles interrupt 0x0b, aka #NP
void handleSegmentNotPresent();

// handle interrupt 0x0c, aka #SS
void handleStackSegmentFault();

// handles interrupt 0x0d, aka #GP
void handleGeneralProtectionFault();

// handles interrupt 0x0e, aka #PF
void handlePageFault();

// handles interrupt 0x10, aka #MF
void handlex87Exception();

// handles interrupt 0x11, aka #AC
void handleAlignmentCheck();

// handles interrupt 0x12, aka #MC
void handleMachineCheck();

// handles interrupt 0x13, aka #XM/#XF
void handleSIMDException();

// handles interrupt 0x14, aka #VE
void handleVirtualisationException();

// handles interrupt 0x15, aka #CP
void handleControlProtectionException();

// handles interrupt 0x1C, aka #HV
void handleHypervisorInjectionException();

// handles interrupt 0x1D, aka #VC
void handleVMMCommunicationException();

// handles interrupt 0x1E, aka #SX
void handleSecurityException();

/**
 * hooks all the exception handlers into the 
 * IDT. `interrupts::configureIDT` **MUST** be 
 * called before this function is called
 * 
 * **/ 
void registerExceptionHandlers();

}
}