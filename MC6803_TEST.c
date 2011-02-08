
/*
	MC6803
	This code creates a MC6803E_MPU_ instance, and then loads a ram image from a file.
	It will then call the code in RAM, until the next instruction is 0x00 (EOC), it will then exit.
*/

#include "MC6803_TEST.h"

int main(int argc, char * argv[])
{
	MC6803E_MPU * p = MC6803E_MPU_Alloc();
	char * ram_file = "RAM.6803";
	char * rom_file = "ROM.6803";
	
	/*if(argc < 2) {
		fprintf(stderr, "usage: %s [-v/-s/-p]\n", argv[0]);
		return 1;
	}*/
	
	// Check for flags, if not set then ask the user what to do.
	int StepThroughEmulation = -1, PrintRegisters = -1, Verbose = -1, DelayExecution = -1;
	int i, j; char t;
	for(i=1; i<argc; i++) {
		// Assume if it's not a flag, it's the ramfile.
		//if((argv[i][0]) != '-') { ram_file = argv[i]; continue; }
		
		j=1; // j is used to itterate each char of each argument.
		while((t = (argv[i][j++]))) {
			switch(t) {
				case 'V':
					Verbose = 1;
					break;
				case 'v':
					Verbose = 0;
					break;
				case 'S':
					StepThroughEmulation = 1;
					break;
				case 's':
					StepThroughEmulation = 0;
					break;
				case 'P':
					PrintRegisters = 1;
					break;
				case 'p':
					PrintRegisters = 0;
					break;
				case 'D':
					DelayExecution = 1;
					break;
				case 'd':
					DelayExecution = 0;
					break;
				default:
					fprintf(stderr, "unknown flag \"-%c\"\n", t);
					break;
			}
		}
	}
	
	if(StepThroughEmulation == -1) {
		StepThroughEmulation = GetResponse('y', 'n', 1, "Step through emulation?");
	}
	if(PrintRegisters == -1) {
		PrintRegisters = GetResponse('y', 'n', 0, "Print registers?");
	}
	if(Verbose == -1) {
		Verbose = GetResponse('y', 'n', 1, "Verbose?");
	}
	if(DelayExecution == -1 && StepThroughEmulation != 1) {
		DelayExecution = GetResponse('y', 'n', 0, "Delay execution?");
	}
	
	uint16_t pcStart;
	Image * image; ImageBlock * block;
	
	// Read the rom_file and load it into memory.
	image = ImageFromFile(rom_file);
	block = image->start;
	pcStart = 0xE000;
	while(block != NULL) {
		MemoryWrite(p, pcStart++, block->val);
		block = block->next;
	}
	FreeImage(image);
	
	// Read the ram_file, and load it into memory.
	image = ImageFromFile(ram_file);
	block = image->start;
	pcStart = 0x4000;
	while(block != NULL) {
		MemoryWrite(p, pcStart++, block->val);
		block = block->next;
	}
	FreeImage(image);
	
	// Start execution at 0x4000.
	//p->pc = 0xE000;
	p->pc = 0x4000;
	
	// The run loop.
	uint8_t instruction;
	while ((instruction = ALU_MC6803E_Fetch(p))) {	// Fetch a byte from the ALU.
		ALU_MC6803E_Execute(p, instruction);		// Execute the instruction.
		
		if((StepThroughEmulation == 1) && (PrintRegisters == 0)) {
			printf("%s", ALU_MC6803E_GetCurrentMneunomic(p));
		} else {
			ALU_MC6803E_PrintCurrentMneunomic(p);
		}
		
		if(DelayExecution == 1) {
			sleep(EXECUTION_DELAY);
		}
		
		if(PrintRegisters == 1) {
			MC6803E_MPU_PrintRegisters(p); 	// Print the state of the registers.
			printf("\n");
		}
		
		if(StepThroughEmulation == 1) {
			if(Verbose == 1) {
				printf("PC:%04X, Press Return to continue execution (or q to quit)... ", p->pc);
			}
			
			// Quit on q.
			if(getchar() == 'q') { break; }
			
			if(Verbose == 1) { printf("\n"); }
		}
	}
	
	if(Verbose == 1) {
		if(instruction == 0x00) {
			// EOC (not supported by the 6803) was reached.
			printf("\nEOC...\n");
		} else {
			// We were terminated before EOC was reached.
			printf("\nTerminating (PC:%04X -> %02X)...\n", p->pc, MemoryRead(p, p->pc));
		}
	} else {
		printf("\n");
	}
	
	// Clean up.
	MC6803E_MPU_Free(p);
	
	return 0;
}

int GetResponse(char char_true, char char_false, int default_resp, char * request)
{
	if(default_resp == 1) {
		printf("%s (%c,%c) ", request, toupper(char_true), char_false);
	} else {
		printf("%s (%c,%c) ", request, char_true, toupper(char_false));
	}
	
	char c = getchar(); if(c!='\n') { while(getchar() != '\n') { ; } }
	if(c == char_true || c == toupper(char_true)) {
		return 1;
	} else if (c == char_false || c == toupper(char_false)) {
		return 0;
	} else if (c == '\n' || c == '\r') {
		return default_resp;
	} else {
		return -1;
	}
}
