#pragma once

namespace nov
{
namespace exception
{

// handles interrupt 0x0e, aka #PF
void handlePageFault();

// handles interrupt 0x0d, aka #GP
void handleGeneralProtectionFault();

// handles interrupt 0x08, aka #DF
void handleDoubleFault();

// handles interrupt 0x0a, aka #TS
void handleInvalidTSS();

// TODO: looots more of these

void registerExceptionHandlers();

}
}