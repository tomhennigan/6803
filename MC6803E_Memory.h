#ifndef __MEMORY_H_INCLUDE
#define __MEMORY_H_INCLUDE

#include "MC6803E_Core.h"

uint8_t MemoryRead(MC6803E_MPU *, uint16_t);
uint8_t MemoryWrite(MC6803E_MPU *, uint16_t, uint8_t);
uint8_t MemoryReadIndexValue(MC6803E_MPU *, uint8_t);
uint8_t MemoryWriteIndexValue(MC6803E_MPU *, uint8_t, uint8_t);

#endif