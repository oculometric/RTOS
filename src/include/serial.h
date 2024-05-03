#pragma once

#include <stdint.h>

#include <stream.h>
#include <io.h>

// TODO: enumify this stuff so it's not just global defines

#define COM1 0x3f8
#define COM2 0x2f8

#define SERIAL_DATA_REG 0
#define SERIAL_INTERRUPT_REG 1
#define SERIAL_INTERRUPTID_REG 2
#define SERIAL_LINECONTROL_REG 3
#define SERIAL_MODEMCONTROL_REG 4
#define SERIAL_LINESTATUS_REG 5
#define SERIAL_MODEMSTATUS_REG 6
#define SERIAL_SCRATCH_REG 7

#define SERIAL_INIT_SUCCESS 0
#define SERIAL_INIT_FAILURE 1

uint8_t initSerial(uint32_t port);

static inline bool transmitReady(uint32_t port) { return inb(port+SERIAL_LINESTATUS_REG) & 0b00100000; }
static inline bool receiveReady(uint32_t port) { return inb(port+SERIAL_LINESTATUS_REG) & 0b00000001; }

void serialPrint(char chr, uint32_t port);

void serialDumpByte(void* start, uint32_t length, uint32_t port, uint8_t per_line = 0, char separator = '\0');

void serialDumpHexByte(void* start, uint32_t length, uint32_t port, uint8_t per_line = 8);
void serialDumpHexWord(void* start, uint32_t length, uint32_t port, uint8_t per_line = 8);
void serialDumpHexDwrd(void* start, uint32_t length, uint32_t port, uint8_t per_line = 8);

extern nov::stream::Stream com_1;
extern nov::stream::Stream com_2;
