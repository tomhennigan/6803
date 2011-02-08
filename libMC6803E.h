
#ifndef __LIB6803_H_INCLUDE
#define __LIB6803_H_INCLUDE

#include <stdint.h>

// MC6803E_MPU_ stuff..
typedef struct _MC6803E_MPU {
	uint16_t pc;
	uint8_t flagRegister;
	uint8_t accumulatorA;
	uint8_t accumulatorB;
	uint16_t * accumulatorD;
	uint16_t indexRegister;
	uint16_t stackPointer;
	uint8_t MemoryMap[0x10000];
} MC6803E_MPU;

// MC6803E_MPU_ Functions
MC6803E_MPU * MC6803E_MPU_Alloc();
void MC6803E_MPU_Free(MC6803E_MPU *);
void MC6803E_MPU_PrintRegisters(MC6803E_MPU *);

// ALU Functions
uint8_t ALU_Fetch(MC6803E_MPU *);
uint16_t ALU_Execute(MC6803E_MPU *, uint8_t instruction);

// Memory Functions
uint8_t MemoryRead(MC6803E_MPU *, uint16_t);
uint8_t MemoryWrite(MC6803E_MPU * p, uint16_t address, uint8_t value);

#endif
