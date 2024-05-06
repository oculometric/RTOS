#include <boot.h>
#include <serial.h>
#include <interrupts.h>
#include <exceptions.h>

// TODO: string splitting
// TODO: interrupts
// TODO: keyboard
// TODO: v-tables
// TODO: timing
// TODO: text wrapping
// FIXME: append single char broken in string

using namespace nov;
using namespace stream;

extern "C" void main(boot::OSHintTable* os_hint_table)
{
    initSerial(COM1);
    com_1 << "hello from kernel main." << endl;
    com_1.flush();

    com_1 << "configuring IDT" << endl;
    interrupts::configureIDT();
    exception::registerExceptionHandlers();
    interrupts::configureIRQs((uint8_t)0x20);
    interrupts::enableInterrupts();
    interrupts::setIRQEnabled(0, true);
    interrupts::setIRQEnabled(1, true);
    //interrupts::setIRQEnabled(2, true);
    //interrupts::setIRQEnabled(3, true);
    //interrupts::setIRQEnabled(4, true);
    //interrupts::setIRQEnabled(5, true);
    //interrupts::setIRQEnabled(6, true);
    //interrupts::setIRQEnabled(7, true);

    //interrupts::setIRQEnabled(8, true);
    //interrupts::setIRQEnabled(9, true);
    //interrupts::setIRQEnabled(10, true);
    //interrupts::setIRQEnabled(12, true);
    //interrupts::setIRQEnabled(13, true);
    //interrupts::setIRQEnabled(14, true);
    //interrupts::setIRQEnabled(15, true);

    com_1 << "all done." << endl;
    com_1.flush();

    while(true);
}