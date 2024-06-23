#pragma once

#include <stdint.h>

#include <stream.h>
#include <io.h>

namespace nov
{
namespace serial
{

enum Port
{
    COM1 = 0x3f8,
    COM2 = 0x2f8
};

enum Subport
{
    SERIAL_DATA_REG,
    SERIAL_INTERRUPT_REG,
    SERIAL_INTERRUPTID_REG,
    SERIAL_LINECONTROL_REG,
    SERIAL_MODEMCONTROL_REG,
    SERIAL_LINESTATUS_REG,
    SERIAL_MODEMSTATUS_REG,
    SERIAL_SCRATCH_REG
};

enum InitStatus
{
    SERIAL_INIT_SUCCESS,
    SERIAL_INIT_FAILURE
};

InitStatus initSerial(Port port);

void serialPrint(char chr, Port port);

void serialDumpByte(void* start, uint32_t length, Port port, uint8_t per_line = 0, char separator = '\0');

void serialDumpHexByte(void* start, uint32_t length, Port port, uint8_t per_line = 8);
void serialDumpHexWord(void* start, uint32_t length, Port port, uint8_t per_line = 8);
void serialDumpHexDwrd(void* start, uint32_t length, Port port, uint8_t per_line = 8);

extern stream::Stream com_1;
extern stream::Stream com_2;

}
}