
#ifndef __MICROPROCESSOR_H_INCLUDE
#define __MICROPROCESSOR_H_INCLUDE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Just in case...
#ifndef uint16_t
	#define uint16_t unsigned short int
#endif
#ifndef uint8_t
	#define uint8_t unsigned char
#endif

typedef struct _MC6803E_MPU {
	uint16_t pc;
	uint8_t flagRegister;
	uint8_t accumulatorA;
	uint8_t accumulatorB;
	uint16_t * accumulatorD;
	uint16_t indexRegister;
	uint16_t stackPointer;
	uint8_t MemoryMap[0x10000];
	// This is used to a pointer to the mneunomic for the last command.
	char * lastCommandMneunomic;
} MC6803E_MPU;

MC6803E_MPU * MC6803E_MPU_Alloc();
void MC6803E_MPU_Free(MC6803E_MPU *);
void MC6803E_MPU_PrintRegisters(MC6803E_MPU *);
char * _hex_ToStringWithLeadingZeros(unsigned int, unsigned int);
char * _uint8_ToString(uint8_t);
char * _uint16_ToString(uint16_t);

uint16_t uint16_From_uint8s(uint8_t, uint8_t);

#endif