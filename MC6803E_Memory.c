
#include "MC6803E_Memory.h"

uint8_t MemoryRead(MC6803E_MPU * p, uint16_t address)
	{ return (uint8_t)(p->MemoryMap[address]); }

uint8_t MemoryWrite(MC6803E_MPU * p, uint16_t address, uint8_t value)
	{ return (uint8_t)(p->MemoryMap[address] = (uint8_t)value); }

uint8_t MemoryReadIndexValue(MC6803E_MPU * p, uint8_t offset)
	{ return (uint8_t)(MemoryRead(p, (p->indexRegister + offset))); }

uint8_t MemoryWriteIndexValue(MC6803E_MPU * p, uint8_t offset, uint8_t value)
	{ return (uint8_t)(MemoryWrite(p, (p->indexRegister + offset), value)); }
