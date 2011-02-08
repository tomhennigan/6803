
#include "MC6803E_ALU.h"

/*
	Debug Instructions
*/

char * ALU_MC6803E_GetCurrentMneunomic(MC6803E_MPU * p)
	{ return p->lastCommandMneunomic; }
void ALU_MC6803E_PrintCurrentMneunomic(MC6803E_MPU * p)
	{ printf("%s\n", ALU_MC6803E_GetCurrentMneunomic(p)); }
void ALU_MC6803E_FreeCurrentMneunomic(MC6803E_MPU * p)
	{ if(p->lastCommandMneunomic != NULL) { free(p->lastCommandMneunomic); } }

char * ALU_MC6803E_SetCurrentMneunomic(MC6803E_MPU * p, char * instruction)
{
	ALU_MC6803E_FreeCurrentMneunomic(p);
	p->lastCommandMneunomic = calloc(128, sizeof(char));
	sprintf(p->lastCommandMneunomic, "%04X:\t%s", p->pc, instruction);
	return p->lastCommandMneunomic;
}
char * ALU_MC6803E_SetCurrentMneunomicWithPayload(MC6803E_MPU * p, char * instruction, unsigned int payload)
{
	ALU_MC6803E_FreeCurrentMneunomic(p);
	p->lastCommandMneunomic = calloc(128, sizeof(char));
	
	char * format_string = calloc(sizeof(char), strlen(instruction) + 0xff);
	sprintf(format_string, "%04X:\t", p->pc);
	strcat(format_string, instruction);
	
	sprintf(p->lastCommandMneunomic, format_string, payload);
	
	free(format_string);
	return p->lastCommandMneunomic;
}

/*
	Convenience Functions.
*/
uint16_t ALU_MC6803E_IncrementPC(MC6803E_MPU * p, uint16_t ammount)
	{ return p->pc += ammount; }

uint8_t ALU_MC6803E_GetFlag(MC6803E_MPU * p, uint8_t flag)
	{ if(p->flagRegister & flag) { return (uint8_t)1; } else { return (uint8_t)0; } }
uint8_t ALU_MC6803E_SetFlag(MC6803E_MPU * p, uint8_t flag)
	{ return (p->flagRegister = (p->flagRegister | flag)); }
uint8_t ALU_MC6803E_UnsetFlag(MC6803E_MPU * p, uint8_t flag)
	{ return (p->flagRegister = (p->flagRegister & ~flag)); }
uint8_t ALU_MC6803E_SetFlagIfZero(MC6803E_MPU * p, uint8_t flag, uint8_t val)
	{ if(val == 0) { return ALU_MC6803E_SetFlag(p, flag); } else { return ALU_MC6803E_UnsetFlag(p, flag); } }
uint8_t ALU_MC6803E_SetFlagIfNonZero(MC6803E_MPU * p, uint8_t flag, uint8_t val)
	{ if(val != 0) { return ALU_MC6803E_SetFlag(p, flag); } else { return ALU_MC6803E_UnsetFlag(p, flag); } }

/*
	Implement the Fetch/Execute cycle.
*/
uint8_t ALU_MC6803E_Fetch(MC6803E_MPU * p)
	{ return (uint8_t)MemoryRead(p, p->pc); }

uint16_t ALU_MC6803E_Execute(MC6803E_MPU * p, uint8_t instruction)
{
	switch (instruction) {
		case 0x8C: // CPX Immediate
		case 0x9C: // CPX Direct
		case 0xAC: // CPX Index
		case 0xBC: // CPX Extended
			ALU_MC6803E_CPX(p);
			break;
		case 0x09: // DEX Inherent
			ALU_MC6803E_DEX(p);
			break;
		case 0x34: // DES Inherent
			ALU_MC6803E_DES(p);
			break;
		case 0x08: // INX Inherent
			ALU_MC6803E_INX(p);
			break;
		case 0x31: // INS Inherent
			ALU_MC6803E_INS(p);
			break;
		case 0xCE: // LDX Immediate
		case 0xDE: // LDX Direct
		case 0xEE: // LDX Index
		case 0xFE: // LDX Extended
			ALU_MC6803E_LDX(p);
			break;
		case 0x8E: // LDS Immediate
		case 0x9E: // LDS Direct
		case 0xAE: // LDS Index
		case 0xBE: // LDS Extended
			ALU_MC6803E_LDS(p);
			break;
		case 0xDF: // STX Direct
		case 0xEF: // STX Index
		case 0xFF: // STX Extended
			ALU_MC6803E_STX(p);
			break;
		case 0x9F: // STS Direct
		case 0xAF: // STS Index
		case 0xBF: // STS Extended
			ALU_MC6803E_STS(p);
			break;
		case 0x35: // TXS Inherent
			ALU_MC6803E_TXS(p);
			break;
		case 0x30: // TSX Inherent
			ALU_MC6803E_TSX(p);
			break;
		case 0x3C: // PSHX Inherent
			ALU_MC6803E_PSHX(p);
			break;
		case 0x38: // PULX Inherent
			ALU_MC6803E_PULX(p);
			break;
		case 0x1B: // ABA Inherent
			ALU_MC6803E_ABA(p);
			break;
		case 0x3A: // ABX Inherent
			ALU_MC6803E_ABX(p);
			break;
		case 0x89: // ADCA Immediate
		case 0x99: // ADCA Direct
		case 0xA9: // ADCA Index
		case 0xB9: // ADCA Extended
			ALU_MC6803E_ADCA(p);
			break;
		case 0xC9: // ADCB Immediate
		case 0xD9: // ADCB Direct
		case 0xE9: // ADCB Index
		case 0xF9: // ADCB Extended
			ALU_MC6803E_ADCB(p);
			break;
		case 0x8B: // ADDA Immediate
		case 0x9B: // ADDA Direct
		case 0xAB: // ADDA Index
		case 0xBB: // ADDA Extended
			ALU_MC6803E_ADDA(p);
			break;
		case 0xCB: // ADDB Immediate
		case 0xDB: // ADDB Direct
		case 0xEB: // ADDB Index
		case 0xFB: // ADDB Extended
			ALU_MC6803E_ADDB(p);
			break;
		case 0xC3: // ADDD Immediate
		case 0xD3: // ADDD Direct
		case 0xE3: // ADDD Index
		case 0xF3: // ADDD Extended
			ALU_MC6803E_ADDD(p);
			break;
		case 0x84: // ANDA Immediate
		case 0x94: // ANDA Direct
		case 0xA4: // ANDA Index
		case 0xB4: // ANDA Extended
			ALU_MC6803E_ANDA(p);
			break;
		case 0xC4: // ANDB Immediate
		case 0xD4: // ANDB Direct
		case 0xE4: // ANDB Index
		case 0xF4: // ANDB Extended
			ALU_MC6803E_ANDB(p);
			break;
		case 0x68: // ASL/LSL Index
		case 0x78: // ASL/LSL Extended
			ALU_MC6803E_ASL(p);
			//ALU_MC6803E_LSL(p);
			break;
		case 0x48: // ASLA/LSLA Inherent
			ALU_MC6803E_ASLA(p);
			//ALU_MC6803E_LSLA(p);
			break;
		case 0x58: // ASLB/LSLB Inherent
			ALU_MC6803E_ASLB(p);
			//ALU_MC6803E_LSLB(p);
			break;
		case 0x05: // ASLD/LSLD Inherent
			ALU_MC6803E_ASLD(p);
			//ALU_MC6803E_LSLD(p);
			break;
		case 0x67: // ASR Index
		case 0x77: // ASR Extended
			ALU_MC6803E_ASR(p);
			break;
		case 0x47: // ASRA Inherent
			ALU_MC6803E_ASRA(p);
			break;
		case 0x57: // ASRB Inherent
			ALU_MC6803E_ASRB(p);
			break;
		case 0x85: // BITA Immediate
		case 0x95: // BITA Direct
		case 0xA5: // BITA Index
		case 0xB5: // BITA Extended
			ALU_MC6803E_BITA(p);
			break;
		case 0xC5: // BITB Immediate
		case 0xD5: // BITB Direct
		case 0xE5: // BITB Index
		case 0xF5: // BITB Extended
			ALU_MC6803E_BITB(p);
			break;
		case 0x11: // CBA Inherent
			ALU_MC6803E_CBA(p);
			break;
		case 0x6F: // CLR Index
		case 0x7F: // CLR Extended
			ALU_MC6803E_CLR(p);
			break;
		case 0x4F: // CLRA Inherent
			ALU_MC6803E_CLRA(p);
			break;
		case 0x5F: // CLRB Inherent
			ALU_MC6803E_CLRB(p);
			break;
		case 0x81: // CMPA Immediate
		case 0x91: // CMPA Direct
		case 0xA1: // CMPA Index
		case 0xB1: // CMPA Extended
			ALU_MC6803E_CMPA(p);
			break;
		case 0xC1: // CMPB Immediate
		case 0xD1: // CMPB Direct
		case 0xE1: // CMPB Index
		case 0xF1: // CMPB Extended
			ALU_MC6803E_CMPB(p);
			break;
		case 0x63: // COM Index
		case 0x73: // COM Extended
			ALU_MC6803E_COM(p);
			break;
		case 0x43: // COMA Inherent
			ALU_MC6803E_COMA(p);
			break;
		case 0x53: // COMB Inherent
			ALU_MC6803E_COMB(p);
			break;
		case 0x19: // DAA Inherent
			ALU_MC6803E_DAA(p);
			break;
		case 0x6A: // DEC Index
		case 0x7A: // DEC Extended
			ALU_MC6803E_DEC(p);
			break;
		case 0x4A: // DECA Inherent
			ALU_MC6803E_DECA(p);
			break;
		case 0x5A: // DECB Inherent
			ALU_MC6803E_DECB(p);
			break;
		case 0x88: // EORA Immediate
		case 0x98: // EORA Direct
		case 0xA8: // EORA Index
		case 0xB8: // EORA Extended
			ALU_MC6803E_EORA(p);
			break;
		case 0xC8: // EORB Immediate
		case 0xD8: // EORB Direct
		case 0xE8: // EORB Index
		case 0xF8: // EORB Extended
			ALU_MC6803E_EORB(p);
			break;
		case 0x6C: // INC Index
		case 0x7C: // INC Extended
			ALU_MC6803E_INC(p);
			break;
		case 0x4C: // INCA Inherent
			ALU_MC6803E_INCA(p);
			break;
		case 0x5C: // INCB Inherent
			ALU_MC6803E_INCB(p);
			break;
		case 0x86: // LDAA Immediate
		case 0x96: // LDAA Direct
		case 0xA6: // LDAA Index
		case 0xB6: // LDAA Extended
			ALU_MC6803E_LDAA(p);
			break;
		case 0xC6: // LDAB Immediate
		case 0xD6: // LDAB Direct
		case 0xE6: // LDAB Index
		case 0xF6: // LDAB Extended
			ALU_MC6803E_LDAB(p);
			break;
		case 0xCC: // LDD Immediate
		case 0xDC: // LDD Direct
		case 0xEC: // LDD Index
		case 0xFC: // LDD Extended
			ALU_MC6803E_LDD(p);
			break;
		case 0x64: // LSR Index
		case 0x74: // LSR Extended
			ALU_MC6803E_LSR(p);
			break;
		case 0x44: // LSRA Inherent
			ALU_MC6803E_LSRA(p);
			break;
		case 0x54: // LSRB Inherent
			ALU_MC6803E_LSRB(p);
			break;
		case 0x04: // LSRD Inherent
			ALU_MC6803E_LSRD(p);
			break;
		case 0x3D: // MUL Inherent
			ALU_MC6803E_MUL(p);
			break;
		case 0x60: // NEG Index
		case 0x70: // NEG Extended
			ALU_MC6803E_NEG(p);
			break;
		case 0x40: // NEGA Inherent
			ALU_MC6803E_NEGA(p);
			break;
		case 0x50: // NEGB Inherent
			ALU_MC6803E_NEGB(p);
			break;
		case 0x01: // NOP Inherent
			ALU_MC6803E_NOP(p);
			break;
		case 0x8A: // ORAA Immediate
		case 0x9A: // ORAA Direct
		case 0xAA: // ORAA Index
		case 0xBA: // ORAA Extended
			ALU_MC6803E_ORAA(p);
			break;
		case 0xCA: // ORAB Immediate
		case 0xDA: // ORAB Direct
		case 0xEA: // ORAB Index
		case 0xFA: // ORAB Extended
			ALU_MC6803E_ORAB(p);
			break;
		case 0x36: // PSHA Inherent
			ALU_MC6803E_PSHA(p);
			break;
		case 0x37: // PSHB Inherent
			ALU_MC6803E_PSHB(p);
			break;
		case 0x32: // PULA Inherent
			ALU_MC6803E_PULA(p);
			break;
		case 0x33: // PULB Inherent
			ALU_MC6803E_PULB(p);
			break;
		case 0x69: // ROL Index
		case 0x79: // ROL Extended
			ALU_MC6803E_ROL(p);
			break;
		case 0x49: // ROLA Inherent
			ALU_MC6803E_ROLA(p);
			break;
		case 0x59: // ROLB Inherent
			ALU_MC6803E_ROLB(p);
			break;
		case 0x66: // ROR Index
		case 0x76: // ROR Extended
			ALU_MC6803E_ROR(p);
			break;
		case 0x46: // RORA Inherent
			ALU_MC6803E_RORA(p);
			break;
		case 0x56: // RORB Inherent
			ALU_MC6803E_RORB(p);
			break;
		case 0x10: // SBA Inherent
			ALU_MC6803E_SBA(p);
			break;
		case 0x82: // SBCA Immediate
		case 0x92: // SBCA Direct
		case 0xA2: // SBCA Index
		case 0xB2: // SBCA Extended
			ALU_MC6803E_SBCA(p);
			break;
		case 0xC2: // SBCB Immediate
		case 0xD2: // SBCB Direct
		case 0xE2: // SBCB Index
		case 0xF2: // SBCB Extended
			ALU_MC6803E_SBCB(p);
			break;
		case 0x97: // STAA Direct
		case 0xA7: // STAA Index
		case 0xB7: // STAA Extended
			ALU_MC6803E_STAA(p);
			break;
		case 0xD7: // STAB Direct
		case 0xE7: // STAB Index
		case 0xF7: // STAB Extended
			ALU_MC6803E_STAB(p);
			break;
		case 0xDD: // STD Direct
		case 0xED: // STD Index
		case 0xFD: // STD Extended
			ALU_MC6803E_STD(p);
			break;
		case 0x80: // SUBA Immediate
		case 0x90: // SUBA Direct
		case 0xA0: // SUBA Index
		case 0xB0: // SUBA Extended
			ALU_MC6803E_SUBA(p);
			break;
		case 0xC0: // SUBB Immediate
		case 0xD0: // SUBB Direct
		case 0xE0: // SUBB Index
		case 0xF0: // SUBB Extended
			ALU_MC6803E_SUBB(p);
			break;
		case 0x83: // SUBD Immediate
		case 0x93: // SUBD Direct
		case 0xA3: // SUBD Index
		case 0xB3: // SUBD Extended
			ALU_MC6803E_SUBD(p);
			break;
		case 0x16: // TAB Inherent
			ALU_MC6803E_TAB(p);
			break;
		case 0x17: // TBA Inherent
			ALU_MC6803E_TBA(p);
			break;
		case 0x6D: // TST Index
		case 0x7D: // TST Extended
			ALU_MC6803E_TST(p);
			break;
		case 0x4D: // TSTA Inherent
			ALU_MC6803E_TSTA(p);
			break;
		case 0x5D: // TSTB Inherent
			ALU_MC6803E_TSTB(p);
			break;
		case 0x20: // BRA Immediate
			ALU_MC6803E_BRA(p);
			break;
		case 0x21: // BRN Immediate
			ALU_MC6803E_BRN(p);
			break;
		case 0x24: // BCC Immediate
			ALU_MC6803E_BCC(p);
			//ALU_MC6803E_BHS(p);
			break;
		case 0x25: // BCS Immediate
			ALU_MC6803E_BCS(p);
			//ALU_MC6803E_BLO(p);
			break;
		case 0x27: // BEQ Immediate
			ALU_MC6803E_BEQ(p);
			break;
		case 0x2C: // BGE Immediate
			ALU_MC6803E_BGE(p);
			break;
		case 0x2E: // BGT Immediate
			ALU_MC6803E_BGT(p);
			break;
		case 0x22: // BHI Immediate
			ALU_MC6803E_BHI(p);
			break;
		case 0x2F: // BLE Immediate
			ALU_MC6803E_BLE(p);
			break;
		case 0x23: // BLS Immediate
			ALU_MC6803E_BLS(p);
			break;
		case 0x2D: // BLT Immediate
			ALU_MC6803E_BLT(p);
			break;
		case 0x2B: // BMI Immediate
			ALU_MC6803E_BMI(p);
			break;
		case 0x26: // BNE Immediate
			ALU_MC6803E_BNE(p);
			break;
		case 0x28: // BVC Immediate
			ALU_MC6803E_BVC(p);
			break;
		case 0x29: // BVS Immediate
			ALU_MC6803E_BVS(p);
			break;
		case 0x2A: // BPL Immediate
			ALU_MC6803E_BPL(p);
			break;
		case 0x8D: // BSR Immediate
			ALU_MC6803E_BSR(p);
			break;
		case 0x6E: // JMP Index
		case 0x7E: // JMP Extended
			ALU_MC6803E_JMP(p);
			break;
		case 0x9D: // JSR Immediate
		case 0xAD: // JSR Index
		case 0xBD: // JSR Extended
			ALU_MC6803E_JSR(p);
			break;
		case 0x3B: // RTI Inherent
			ALU_MC6803E_RTI(p);
			break;
		case 0x39: // RTS Inherent
			ALU_MC6803E_RTS(p);
			break;
		case 0x3F: // SWI Inherent
			ALU_MC6803E_SWI(p);
			break;
		case 0x3E: // WAI Inherent
			ALU_MC6803E_WAI(p);
			break;
		case 0x4E: // TESTPC Inherent
			ALU_MC6803E_TESTPC(p);
			break;
		case 0x0C: // CLC Inherent
			ALU_MC6803E_CLC(p);
			break;
		case 0x0E: // CLI Inherent
			ALU_MC6803E_CLI(p);
			break;
		case 0x0A: // CLV Inherent
			ALU_MC6803E_CLV(p);
			break;
		case 0x0D: // SEC Inherent
			ALU_MC6803E_SEC(p);
			break;
		case 0x0F: // SEI Inherent
			ALU_MC6803E_SEI(p);
			break;
		case 0x0B: // SEV Inherent
			ALU_MC6803E_SEV(p);
			break;
		case 0x06: // TAP Inherent
			ALU_MC6803E_TAP(p);
			break;
		case 0x07: // TPA Inherent
			ALU_MC6803E_TPA(p);
			break;
		default:
			printf("Unknown instruction (%X) at PC -> %X.\n", instruction, p->pc);
			break;
	}
	
	// Return the incremented program counter.
	return (uint16_t)ALU_MC6803E_IncrementPC(p, 1);
}

/*
	This is the instruction set. Each command is responsible for the execute cycle, the program counter will be incremented by one byte after each command.
*/

/*
		void ALU_MC6803E_CPX(MC6803E_MPU * p)
		Boolean:	X - M:(M + 1)
		Flags:		N Z V C
*/
void ALU_MC6803E_CPX(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint16_t result;

	switch (instruction) {
		case 0x8C: // CPX Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CPX #$%02X", unsigned_payload);
			result = (p->indexRegister - (uint16_t)unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x9C: // CPX Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CPX %02X", unsigned_payload);
			result = (p->indexRegister - MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xAC: // CPX Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CPX %02X(x)", unsigned_payload);
			result = (p->indexRegister - MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xBC: // CPX Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CPX %04X", unsigned_payload_double);
			result = (p->indexRegister - MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (result & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, (result & 0xff));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_V, (result > 0xff));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, (result & 0x100));
}

/*
		void ALU_MC6803E_DEX(MC6803E_MPU * p)
		Boolean:	X - 1 -> X
		Flags:		Z
*/
void ALU_MC6803E_DEX(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x09: // DEX Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "DEX");
			p->indexRegister--;
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->indexRegister);
}

/*
		void ALU_MC6803E_DES(MC6803E_MPU * p)
		Boolean:	SP - 1 -> SP
		Flags:		
*/
void ALU_MC6803E_DES(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x34: // DES Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "DES");
			p->stackPointer--;
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_INX(MC6803E_MPU * p)
		Boolean:	X + 1 -> X
		Flags:		Z
*/
void ALU_MC6803E_INX(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x08: // INX Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "INX");
			p->indexRegister++;
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->indexRegister);
}

/*
		void ALU_MC6803E_INS(MC6803E_MPU * p)
		Boolean:	SP + 1 -> SP
		Flags:		
*/
void ALU_MC6803E_INS(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x31: // INS Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "INS");
			p->stackPointer++;
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_LDX(MC6803E_MPU * p)
		Boolean:	M -> Xh; (M + 1) -> Xl
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_LDX(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0xCE: // LDX Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDX #$%02X", unsigned_payload);
			p->indexRegister = ((uint16_t)unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xDE: // LDX Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDX %02X", unsigned_payload);
			p->indexRegister = (MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xEE: // LDX Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDX %02X(x)", unsigned_payload);
			p->indexRegister = (MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xFE: // LDX Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDX %04X", unsigned_payload_double);
			p->indexRegister = (MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->indexRegister & 0x8000));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->indexRegister);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
}

/*
		void ALU_MC6803E_LDS(MC6803E_MPU * p)
		Boolean:	M -> SPh; (M + 1) -> SPl
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_LDS(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x8E: // LDS Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDS #$%02X", unsigned_payload);
			p->stackPointer = ((uint16_t)unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x9E: // LDS Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDS %02X", unsigned_payload);
			p->stackPointer = (MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xAE: // LDS Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDS %02X(x)", unsigned_payload);
			p->stackPointer = (MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xBE: // LDS Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDS %04X", unsigned_payload_double);
			p->stackPointer = (MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->stackPointer & 0x8000));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->stackPointer);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
}

/*
		void ALU_MC6803E_STX(MC6803E_MPU * p)
		Boolean:	Xh -> M; Xl -> (M + 1)
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_STX(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint8_t xHigh, xLow;
		xHigh = (p->indexRegister >> 8);
		xLow = (p->indexRegister & 0xff);

	switch (instruction) {
		case 0xDF: // STX Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STX %02X", unsigned_payload);
			MemoryWrite(p, direct_address, xHigh);
			MemoryWrite(p, (direct_address + 1), xLow);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xEF: // STX Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STX %02X(x)", unsigned_payload);
			MemoryWrite(p, MemoryReadIndexValue(p, unsigned_payload), xHigh);
			MemoryWrite(p, (MemoryReadIndexValue(p, unsigned_payload) + 1), xLow);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xFF: // STX Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STX %04X", unsigned_payload_double);
			MemoryWrite(p, unsigned_payload_double, xHigh);
			MemoryWrite(p, unsigned_payload_double, xLow);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	// @CC: Can either part be negative to set the flag? (Bitmask: 0x8080)
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->indexRegister & 0x8000));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->indexRegister);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
}

/*
		void ALU_MC6803E_STS(MC6803E_MPU * p)
		Boolean:	SPh -> M; SPl -> (M + 1)
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_STS(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint8_t spHigh, spLow;
		spHigh = (p->stackPointer >> 8);
		spLow = (p->stackPointer & 0xff);

	switch (instruction) {
		case 0x9F: // STS Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STS %02X", unsigned_payload);
			MemoryWrite(p, direct_address, spHigh);
			MemoryWrite(p, (direct_address + 1), spLow);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xAF: // STS Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STS %02X(x)", unsigned_payload);
			MemoryWrite(p, MemoryReadIndexValue(p, unsigned_payload), spHigh);
			MemoryWrite(p, (MemoryReadIndexValue(p, unsigned_payload) + 1), spLow);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xBF: // STS Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STS %04X", unsigned_payload_double);
			MemoryWrite(p, unsigned_payload_double, spHigh);
			MemoryWrite(p, (unsigned_payload_double + 1), spLow);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->indexRegister & 0x8000));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->indexRegister);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
}

/*
		void ALU_MC6803E_TXS(MC6803E_MPU * p)
		Boolean:	X - 1 -> SP
		Flags:		
*/
void ALU_MC6803E_TXS(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x35: // TXS Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "TXS");
			p->stackPointer = (p->indexRegister - 1);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_TSX(MC6803E_MPU * p)
		Boolean:	SP + 1 -> X
		Flags:		
*/
void ALU_MC6803E_TSX(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x30: // TSX Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "TSX");
			p->indexRegister = (p->stackPointer + 1);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_PSHX(MC6803E_MPU * p)
		Boolean:	Xl -> Msp; SP - 1 -> SP
		Flags:		
*/
void ALU_MC6803E_PSHX(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x3C: // PSHX Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "PSHX");
			p->indexRegister = ((p->indexRegister & 0xff00) | MemoryRead(p, p->stackPointer));
			break;
		default:
			break;
	}
	
	p->stackPointer--;
}

/*
		void ALU_MC6803E_PULX(MC6803E_MPU * p)
		Boolean:	SP +1 -> SP; Msp -> Xh; SP + 1 -> SP; Msp -> Xl
		Flags:		
*/
void ALU_MC6803E_PULX(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x38: // PULX Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "PULX");
			p->stackPointer++;
			p->indexRegister = (uint16_t)MemoryRead(p, p->stackPointer);
			p->indexRegister = ((p->indexRegister << 8) & 0xff00);
			p->stackPointer++;
			p->indexRegister = (p->indexRegister | (uint16_t)MemoryRead(p, p->stackPointer));
			break;
		default:
			break;
	}
	
}

/*
		void ALU_MC6803E_ABA(MC6803E_MPU * p)
		Boolean:	A + B -> A
		Flags:		H N Z V C
*/
void ALU_MC6803E_ABA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint16_t result = (p->accumulatorA + p->accumulatorB);

	switch (instruction) {
		case 0x1B: // ABA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "ABA");
			
			p->accumulatorA = (uint8_t)result;
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_H, (p->accumulatorA & 0x10));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorA & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_V, (result > 0xff));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, (result & 0x100));
}

/*
		void ALU_MC6803E_ABX(MC6803E_MPU * p)
		Boolean:	00:B + X -> X
		Flags:		
*/
void ALU_MC6803E_ABX(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x3A: // ABX Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "ABX");
			p->indexRegister = (p->indexRegister + (uint16_t)p->accumulatorB);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_ADCA(MC6803E_MPU * p)
		Boolean:	A + M + C -> A
		Flags:		H N Z V C
*/
void ALU_MC6803E_ADCA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint16_t new_value; // We use uint16_t to see if we overflow in a uint8_t.

	switch (instruction) {
		case 0x89: // ADCA Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADCA #$%02X", unsigned_payload);
			new_value = (p->accumulatorA + unsigned_payload + ALU_MC6803E_GetFlag(p, MC6803E_FLAG_C));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x99: // ADCA Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADCA %02X", unsigned_payload);
			new_value = (p->accumulatorA + MemoryRead(p, direct_address) + ALU_MC6803E_GetFlag(p, MC6803E_FLAG_C));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xA9: // ADCA Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADCA %02X(x)", unsigned_payload);
			new_value = (p->accumulatorA + MemoryRead(p, p->indexRegister + unsigned_payload) + ALU_MC6803E_GetFlag(p, MC6803E_FLAG_C));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xB9: // ADCA Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADCA %04X", unsigned_payload_double);
			new_value = (p->accumulatorA + MemoryRead(p, unsigned_payload_double) + ALU_MC6803E_GetFlag(p, MC6803E_FLAG_C));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	p->accumulatorA = (uint8_t)new_value;

	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_H, (p->accumulatorA & 0x10));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorA & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_V, (new_value > 0xff));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, (p->accumulatorA & 0x100));
}

/*
		void ALU_MC6803E_ADCB(MC6803E_MPU * p)
		Boolean:	B + M + C -> B
		Flags:		H N Z V C
*/
void ALU_MC6803E_ADCB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint16_t new_value; // We use uint16_t to see if we overflow in a uint8_t.
	
	switch (instruction) {
		case 0xC9: // ADCB Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADCB #$%02X", unsigned_payload);
			new_value = (p->accumulatorB + unsigned_payload + ALU_MC6803E_GetFlag(p, MC6803E_FLAG_C));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xD9: // ADCB Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADCB %02X", unsigned_payload);
			new_value = (p->accumulatorB + MemoryRead(p, direct_address) + ALU_MC6803E_GetFlag(p, MC6803E_FLAG_C));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xE9: // ADCB Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADCB %02X(x)", unsigned_payload);
			new_value = (p->accumulatorB + MemoryRead(p, p->indexRegister + unsigned_payload) + ALU_MC6803E_GetFlag(p, MC6803E_FLAG_C));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xF9: // ADCB Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADCB %04X", unsigned_payload_double);
			new_value = (p->accumulatorB + MemoryRead(p, unsigned_payload_double) + ALU_MC6803E_GetFlag(p, MC6803E_FLAG_C));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	p->accumulatorB = (uint8_t)new_value;

	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_H, (p->accumulatorB & 0x10));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorB & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorB);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_V, (new_value > 0xff)); // new_value is a uint16_t.
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, (p->accumulatorB & 0x100));
}

/*
		void ALU_MC6803E_ADDA(MC6803E_MPU * p)
		Boolean:	A + M -> A
		Flags:		H N Z V C
*/
void ALU_MC6803E_ADDA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint16_t new_value;

	switch (instruction) {
		case 0x8B: // ADDA Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADDA #$%02X", unsigned_payload);
			new_value = (p->accumulatorA + unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x9B: // ADDA Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADDA %02X", unsigned_payload);
			new_value = (p->accumulatorA + MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xAB: // ADDA Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADDA %02X(x)", unsigned_payload);
			new_value = (p->accumulatorA + MemoryRead(p, p->indexRegister + unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xBB: // ADDA Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADDA %04X", unsigned_payload_double);
			new_value = (p->accumulatorA + MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	p->accumulatorA = (uint8_t)new_value;

	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_H, (p->accumulatorA & 0x10));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorA & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_V, (new_value > 0xff));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, (p->accumulatorA & 0x100));
}

/*
		void ALU_MC6803E_ADDB(MC6803E_MPU * p)
		Boolean:	B + M -> A
		Flags:		H N Z V C
*/
void ALU_MC6803E_ADDB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint16_t new_value;

	switch (instruction) {
		case 0xCB: // ADDB Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADDB #$%02X", unsigned_payload);
			new_value = (p->accumulatorB + unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xDB: // ADDB Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADDB %02X", unsigned_payload);
			new_value = (p->accumulatorB + MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xEB: // ADDB Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADDB %02X(x)", unsigned_payload);
			new_value = (p->accumulatorB + MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xFB: // ADDB Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADDB %04X", unsigned_payload_double);
			new_value = (p->accumulatorB + MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	p->accumulatorB = (uint8_t)new_value;

	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_H, (p->accumulatorB & 0x10));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorB & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorB);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_V, (new_value > 0xff));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, (p->accumulatorB & 0x100));
}

/*
		void ALU_MC6803E_ADDD(MC6803E_MPU * p)
		Boolean:	D + M:(M + 1) -> D
		Flags:		N Z V C
*/
void ALU_MC6803E_ADDD(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint32_t new_value;

	switch (instruction) {
		case 0xC3: // ADDD Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADDD #$%02X", unsigned_payload);
			new_value = (*(p->accumulatorD) + unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xD3: // ADDD Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADDD %02X", unsigned_payload);
			new_value = (*(p->accumulatorD) + MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xE3: // ADDD Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADDD %02X(x)", unsigned_payload);
			new_value = (*(p->accumulatorD) + MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xF3: // ADDD Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ADDD %04X", unsigned_payload_double);
			new_value = (*(p->accumulatorD) + MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	*(p->accumulatorD) = (uint16_t)new_value;

	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (*(p->accumulatorD) & 0x8000));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, *(p->accumulatorD));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_V, (new_value > 0xffff));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, (*(p->accumulatorD) & 0x10000));
}

/*
		void ALU_MC6803E_ANDA(MC6803E_MPU * p)
		Boolean:	A & M -> A
		Flags:		N Z (0 -> V) C
*/
void ALU_MC6803E_ANDA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x84: // ANDA Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ANDA #$%02X", unsigned_payload);
			p->accumulatorA = (p->accumulatorA & unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x94: // ANDA Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ANDA %02X", unsigned_payload);
			p->accumulatorA = (p->accumulatorA & MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xA4: // ANDA Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ANDA %02X(x)", unsigned_payload);
			p->accumulatorA = (p->accumulatorA & MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xB4: // ANDA Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ANDA %04X", unsigned_payload_double);
			p->accumulatorA = (p->accumulatorA & MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorA & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
	// @CC Why do you carry with and? surely 0xff & 0xff is just 0xff.... (no carry?)
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_C);
}

/*
		void ALU_MC6803E_ANDB(MC6803E_MPU * p)
		Boolean:	B & M -> B
		Flags:		N Z (0 -> V) C
*/
void ALU_MC6803E_ANDB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0xC4: // ANDB Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ANDB #$%02X", unsigned_payload);
			p->accumulatorB = (p->accumulatorB & unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xD4: // ANDB Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ANDB %02X", unsigned_payload);
			p->accumulatorB = (p->accumulatorB & MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xE4: // ANDB Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ANDB %02X(x)", unsigned_payload);
			p->accumulatorB = (p->accumulatorB & MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xF4: // ANDB Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ANDB %04X", unsigned_payload_double);
			p->accumulatorB = (p->accumulatorB & MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorB & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorB);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
	// @CC see ANDA re carry
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_C);
}

/*
		void ALU_MC6803E_ASL(MC6803E_MPU * p)
		Boolean:	b7 -> C; b6 b5 b4 b3 b2 b1 b0 0 -> M
		Flags:		N Z V C
*/
void ALU_MC6803E_ASL(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint8_t overflow, carry, value;

	switch (instruction) {
		case 0x68: // ASL Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ASL %02X(x)", unsigned_payload);
			value = MemoryReadIndexValue(p, unsigned_payload);
			overflow = (value & 0x80);
			value = ((value << 1) & 0xfe);
			MemoryWriteIndexValue(p, unsigned_payload, value);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x78: // ASL Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ASL %04X", unsigned_payload_double);
			value = MemoryRead(p, unsigned_payload_double);
			overflow = (value & 0x80);
			value = ((value << 1) & 0xfe);
			MemoryWrite(p, unsigned_payload_double, value);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (value & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, value);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_V, overflow);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, carry);
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_ASLA(MC6803E_MPU * p)
		Boolean:	b7 -> C; b6 b5 b4 b3 b2 b1 b0 0 -> A
		Flags:		N Z V C
*/
void ALU_MC6803E_ASLA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x48: // ASLA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "ASLA");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_ASLB(MC6803E_MPU * p)
		Boolean:	b7 -> C; b6 b5 b4 b3 b2 b1 b0 0 -> B
		Flags:		N Z V C
*/
void ALU_MC6803E_ASLB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x58: // ASLB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "ASLB");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_ASLD(MC6803E_MPU * p)
		Boolean:	b15 -> C; b14 b13 b12 b11 ... b3 b2 b1 b0 0 -> D
		Flags:		N Z V C
*/
void ALU_MC6803E_ASLD(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x05: // ASLD Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "ASLD");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_ASR(MC6803E_MPU * p)
		Boolean:	b0 -> C; b7 b7 b6 b5 b4 b3 b2 b1 -> M;
		Flags:		N Z V C
*/
void ALU_MC6803E_ASR(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x67: // ASR Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ASR %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x77: // ASR Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ASR %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_ASRA(MC6803E_MPU * p)
		Boolean:	b0 -> C; b7 b7 b6 b5 b4 b3 b2 b1 -> A;
		Flags:		N Z V C
*/
void ALU_MC6803E_ASRA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x47: // ASRA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "ASRA");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_ASRB(MC6803E_MPU * p)
		Boolean:	b0 -> C; b7 b7 b6 b5 b4 b3 b2 b1 -> B;
		Flags:		N Z V C
*/
void ALU_MC6803E_ASRB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x57: // ASRB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "ASRB");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BITA(MC6803E_MPU * p)
		Boolean:	A & M
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_BITA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x85: // BITA Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BITA #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x95: // BITA Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BITA %02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xA5: // BITA Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BITA %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xB5: // BITA Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BITA %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BITB(MC6803E_MPU * p)
		Boolean:	B & M
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_BITB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0xC5: // BITB Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BITB #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xD5: // BITB Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BITB %02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xE5: // BITB Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BITB %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xF5: // BITB Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BITB %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_CBA(MC6803E_MPU * p)
		Boolean:	A - B
		Flags:		N Z V C
*/
void ALU_MC6803E_CBA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x11: // CBA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "CBA");
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_CLR(MC6803E_MPU * p)
		Boolean:	00 -> M
		Flags:		(0 -> N) (1 -> Z) (0 -> V) (0 -> C)
*/
void ALU_MC6803E_CLR(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x6F: // CLR Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CLR %02X(x)", unsigned_payload);
			MemoryWriteIndexValue(p, unsigned_payload, 0x00);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x7F: // CLR Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CLR %04X", unsigned_payload_double);
			MemoryWrite(p, unsigned_payload_double, 0x00);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_N);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_Z);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_C);
}

/*
		void ALU_MC6803E_CLRA(MC6803E_MPU * p)
		Boolean:	00 -> A
		Flags:		(0 -> N) (1 -> Z) (0 -> V) (0 -> C)
*/
void ALU_MC6803E_CLRA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x4F: // CLRA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "CLRA");
			p->accumulatorA = 0x00;
			break;
		default:
			break;
	}
	
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_N);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_Z);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_C);
}

/*
		void ALU_MC6803E_CLRB(MC6803E_MPU * p)
		Boolean:	00 -> B
		Flags:		(0 -> N) (1 -> Z) (0 -> V) (0 -> C)
*/
void ALU_MC6803E_CLRB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x5F: // CLRB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "CLRB");
			p->accumulatorB = 0x00;
			break;
		default:
			break;
	}
	
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_N);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_Z);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_C);
}

/*
		void ALU_MC6803E_CMPA(MC6803E_MPU * p)
		Boolean:	A - M
		Flags:		N Z V C
*/
void ALU_MC6803E_CMPA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;
	
	uint16_t result;
	
	switch (instruction) {
		case 0x81: // CMPA Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CMPA #$%02X", unsigned_payload);
			result = (p->accumulatorA - unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x91: // CMPA Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CMPA %02X", unsigned_payload);
			result = (p->accumulatorA - MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xA1: // CMPA Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CMPA %02X(x)", unsigned_payload);
			result = (p->accumulatorA - MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xB1: // CMPA Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CMPA %04X", unsigned_payload_double);
			result = (p->accumulatorA - MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (result & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, result);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_V, (result > 0xff));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, (result & 0x100));
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_CMPB(MC6803E_MPU * p)
		Boolean:	B - M
		Flags:		N Z V C
*/
void ALU_MC6803E_CMPB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint16_t result;

	switch (instruction) {
		case 0xC1: // CMPB Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CMPB #$%02X", unsigned_payload);
			result = (p->accumulatorB - unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xD1: // CMPB Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CMPB %02X", unsigned_payload);
			result = (p->accumulatorB - MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xE1: // CMPB Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CMPB %02X(x)", unsigned_payload);
			result = (p->accumulatorB - MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xF1: // CMPB Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "CMPB %04X", unsigned_payload_double);
			result = (p->accumulatorB - MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (result & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, result);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_V, (result > 0xff));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, (result & 0x100));
}

/*
		void ALU_MC6803E_COM(MC6803E_MPU * p)
		Boolean:	~M -> M
		Flags:		N Z (0 -> V) (1 -> C)
*/
void ALU_MC6803E_COM(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint8_t result;

	switch (instruction) {
		case 0x63: // COM Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "COM %02X(x)", unsigned_payload);
			result = MemoryWriteIndexValue(p, unsigned_payload, (~MemoryReadIndexValue(p, unsigned_payload)));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x73: // COM Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "COM %04X", unsigned_payload_double);
			result = MemoryWrite(p, unsigned_payload_double, (~MemoryRead(p, unsigned_payload_double)));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (result & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, result);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
	ALU_MC6803E_SetFlag(p, MC6803E_FLAG_C);
}

/*
		void ALU_MC6803E_COMA(MC6803E_MPU * p)
		Boolean:	~A -> A
		Flags:		N Z (0 -> V) (1 -> C)
*/
void ALU_MC6803E_COMA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x43: // COMA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "COMA");
			p->accumulatorA = (~p->accumulatorA);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorA & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
	ALU_MC6803E_SetFlag(p, MC6803E_FLAG_C);
}

/*
		void ALU_MC6803E_COMB(MC6803E_MPU * p)
		Boolean:	~B -> B
		Flags:		N Z (0 -> V) (1 -> C)
*/
void ALU_MC6803E_COMB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x53: // COMB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "COMB");
			p->accumulatorB = (~p->accumulatorB);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorB & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorB);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
	ALU_MC6803E_SetFlag(p, MC6803E_FLAG_C);
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_DAA(MC6803E_MPU * p)
		Boolean:	BCD(Ah) + BCD(Al) -> A
		Flags:		N Z V C
*/
void ALU_MC6803E_DAA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x19: // DAA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "DAA");
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_DEC(MC6803E_MPU * p)
		Boolean:	M - 1 -> M
		Flags:		N Z V
*/
void ALU_MC6803E_DEC(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint16_t result;

	switch (instruction) {
		case 0x6A: // DEC Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "DEC %02X(x)", unsigned_payload);
			result = (MemoryWriteIndexValue(p, unsigned_payload, (MemoryReadIndexValue(p, unsigned_payload) - 1)));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x7A: // DEC Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "DEC %04X", unsigned_payload_double);
			result = (MemoryWrite(p, unsigned_payload_double, (MemoryRead(p, unsigned_payload_double) - 1)));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (result & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, (result & 0xff));
	// @CC how can this overflow...
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_V, (result > 0xff));
}

/*
		void ALU_MC6803E_DECA(MC6803E_MPU * p)
		Boolean:	A - 1 -> A
		Flags:		N Z V
*/
void ALU_MC6803E_DECA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x4A: // DECA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "DECA");
			p->accumulatorA--;
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorA & 0x80));
	// @CC how can this overlow... see dec
}

/*
		void ALU_MC6803E_DECB(MC6803E_MPU * p)
		Boolean:	B - 1 -> B
		Flags:		N Z V
*/
void ALU_MC6803E_DECB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x5A: // DECB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "DECB");
			p->accumulatorB--;
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorB);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorB & 0x80));
	// @CC how can this overflow... (see deca)
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_EORA(MC6803E_MPU * p)
		Boolean:	A ^ M -> A
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_EORA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x88: // EORA Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "EORA #$%02X", unsigned_payload);
			p->accumulatorA = (p->accumulatorA ^ unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x98: // EORA Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "EORA %02X", unsigned_payload);
			p->accumulatorA = (p->accumulatorA ^ MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xA8: // EORA Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "EORA %02X(x)", unsigned_payload);
			p->accumulatorA = (p->accumulatorA ^ MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xB8: // EORA Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "EORA %04X", unsigned_payload_double);
			p->accumulatorA = (p->accumulatorA ^ MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorA & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
}

/*
		void ALU_MC6803E_EORB(MC6803E_MPU * p)
		Boolean:	B ^ M -> B
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_EORB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0xC8: // EORB Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "EORB #$%02X", unsigned_payload);
			p->accumulatorB = (p->accumulatorB ^ unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xD8: // EORB Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "EORB %02X", unsigned_payload);
			p->accumulatorB = (p->accumulatorB ^ MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xE8: // EORB Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "EORB %02X(x)", unsigned_payload);
			p->accumulatorB = (p->accumulatorB ^ MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xF8: // EORB Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "EORB %04X", unsigned_payload_double);
			p->accumulatorB = (p->accumulatorB ^ MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorB & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorB);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_INC(MC6803E_MPU * p)
		Boolean:	M + 1 -> M
		Flags:		N Z V
*/
void ALU_MC6803E_INC(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x6C: // INC Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "INC %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x7C: // INC Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "INC %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_INCA(MC6803E_MPU * p)
		Boolean:	A + 1 -> A
		Flags:		N Z V
*/
void ALU_MC6803E_INCA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x4C: // INCA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "INCA");
			p->accumulatorA++;
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, p->accumulatorA);
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_V, p->accumulatorA);
}

/*
		void ALU_MC6803E_INCB(MC6803E_MPU * p)
		Boolean:	B + 1 -> B
		Flags:		N Z V
*/
void ALU_MC6803E_INCB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x5C: // INCB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "INCB");
			p->accumulatorB++;
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, p->accumulatorB);
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorB);
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_V, p->accumulatorB);
}

/*
		void ALU_MC6803E_LDAA(MC6803E_MPU * p)
		Boolean:	M -> A
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_LDAA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x86: // LDAA Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDAA #$%02X", unsigned_payload);
			p->accumulatorA = unsigned_payload;
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x96: // LDAA Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDAA %02X", unsigned_payload);
			p->accumulatorA = MemoryRead(p, direct_address);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xA6: // LDAA Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDAA %02X(x)", unsigned_payload);
			p->accumulatorA = MemoryReadIndexValue(p, unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xB6: // LDAA Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDAA %04X", unsigned_payload_double);
			p->accumulatorA = MemoryRead(p, unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorA & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
}

/*
		void ALU_MC6803E_LDAB(MC6803E_MPU * p)
		Boolean:	M -> B
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_LDAB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0xC6: // LDAB Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDAB #$%02X", unsigned_payload);
			p->accumulatorB = unsigned_payload;
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xD6: // LDAB Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDAB %02X", unsigned_payload);
			p->accumulatorB = MemoryRead(p, direct_address);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xE6: // LDAB Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDAB %02X(x)", unsigned_payload);
			p->accumulatorB = MemoryReadIndexValue(p, unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xF6: // LDAB Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDAB %04X", unsigned_payload_double);
			p->accumulatorB = MemoryRead(p, unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorB & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorB);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
}

/*
		void ALU_MC6803E_LDD(MC6803E_MPU * p)
		Boolean:	M:(M + 1) -> D
		Flags:		H I N Z V C
*/
void ALU_MC6803E_LDD(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0xCC: // LDD Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDD #$%02X", unsigned_payload);
			*(p->accumulatorD) = (uint16_t)unsigned_payload;
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xDC: // LDD Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDD %02X", unsigned_payload);
			*(p->accumulatorD) = uint16_From_uint8s(MemoryRead(p, direct_address), MemoryRead(p, (direct_address + 1)));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xEC: // LDD Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDD %02X(x)", unsigned_payload);
			*(p->accumulatorD) = uint16_From_uint8s(MemoryReadIndexValue(p, unsigned_payload), MemoryReadIndexValue(p, (unsigned_payload + 1)));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xFC: // LDD Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LDD %04X", unsigned_payload_double);
			*(p->accumulatorD) = unsigned_payload_double;
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_H, (*(p->accumulatorD) & 0x0100));
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (*(p->accumulatorD) & 0x0100));
	// @CC why set I?
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, *(p->accumulatorD));
	// @CC why set V?
	// @CC why set C?
}

// The same as ASL.
//void ALU_MC6803E_LSL(MC6803E_MPU * p) { ; }
//void ALU_MC6803E_LSLA(MC6803E_MPU * p) { ; }
//void ALU_MC6803E_LSLB(MC6803E_MPU * p) { ; }
//void ALU_MC6803E_LSLD(MC6803E_MPU * p) { ; }

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_LSR(MC6803E_MPU * p)
		Boolean:	b0 -> C; 0 b7 b6 b5 b4 b3 b2 b1 -> M
		Flags:		(0 -> N) Z V C
*/
void ALU_MC6803E_LSR(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x64: // LSR Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LSR %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x74: // LSR Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "LSR %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_LSRA(MC6803E_MPU * p)
		Boolean:	b0 -> C; 0 b7 b6 b5 b4 b3 b2 b1 -> A
		Flags:		(0 -> N) Z V C
*/
void ALU_MC6803E_LSRA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x44: // LSRA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "LSRA");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_LSRB(MC6803E_MPU * p)
		Boolean:	b0 -> C; 0 b7 b6 b5 b4 b3 b2 b1 -> B
		Flags:		(0 -> N) Z V C
*/
void ALU_MC6803E_LSRB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x54: // LSRB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "LSRB");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_LSRD(MC6803E_MPU * p)
		Boolean:	b0 -> C; 0 b16 b15 b14 ... b3 b2 b1 -> D
		Flags:		(0 -> N) Z V C
*/
void ALU_MC6803E_LSRD(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x04: // LSRD Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "LSRD");
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_MUL(MC6803E_MPU * p)
		Boolean:	A * B -> D
		Flags:		C
*/
void ALU_MC6803E_MUL(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	
	// The instruction set says set the carry flag, but 0xff * 0xff is only 0xfe01 which doesn't carry...
	if((((int)p->accumulatorA * (int)p->accumulatorB) & 0xffff0000)) {
		ALU_MC6803E_SetFlag(p, MC6803E_FLAG_C);
	}
	
	switch (instruction) {
		case 0x3D: // MUL Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "MUL");
			*(p->accumulatorD) = ((uint16_t)p->accumulatorA * (uint16_t)p->accumulatorB);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_NEG(MC6803E_MPU * p)
		Boolean:	00 - M -> M
		Flags:		N Z V C
*/
void ALU_MC6803E_NEG(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x60: // NEG Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "NEG %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x70: // NEG Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "NEG %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_NEGA(MC6803E_MPU * p)
		Boolean:	00 - A -> A
		Flags:		N Z V C
*/
void ALU_MC6803E_NEGA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x40: // NEGA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "NEGA");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_NEGB(MC6803E_MPU * p)
		Boolean:	00 - B -> B
		Flags:		N Z V C
*/
void ALU_MC6803E_NEGB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x50: // NEGB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "NEGB");
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_NOP(MC6803E_MPU * p)
		Boolean:	PC + 1 -> PC
		Flags:		
*/
void ALU_MC6803E_NOP(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x01: // NOP Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "NOP");
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_ORRA(MC6803E_MPU * p)
		Boolean:	A | M -> A
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_ORAA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x8A: // ORAA Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ORAA #$%02X", unsigned_payload);
			p->accumulatorA = (p->accumulatorA | unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x9A: // ORAA Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ORAA %02X", unsigned_payload);
			p->accumulatorA = (p->accumulatorA | MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xAA: // ORAA Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ORAA %02X(x)", unsigned_payload);
			p->accumulatorA = (p->accumulatorA | MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xBA: // ORAA Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ORAA %04X", unsigned_payload_double);
			p->accumulatorA = (p->accumulatorA | MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorA & 0x80));
}

/*
		void ALU_MC6803E_ORRB(MC6803E_MPU * p)
		Boolean:	B | M -> B
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_ORAB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0xCA: // ORAB Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ORAB #$%02X", unsigned_payload);
			p->accumulatorB = (p->accumulatorB | unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xDA: // ORAB Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ORAB %02X", unsigned_payload);
			p->accumulatorB = (p->accumulatorB | MemoryRead(p, direct_address));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xEA: // ORAB Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ORAB %02X(x)", unsigned_payload);
			p->accumulatorB = (p->accumulatorB | MemoryReadIndexValue(p, unsigned_payload));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xFA: // ORAB Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ORAB %04X", unsigned_payload_double);
			p->accumulatorB = (p->accumulatorB | MemoryRead(p, unsigned_payload_double));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorB);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorB & 0x80));
}

/*
		void ALU_MC6803E_PSHA(MC6803E_MPU * p)
		Boolean:	A -> Stack
		Flags:		
*/
void ALU_MC6803E_PSHA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x36: // PSHA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "PSHA");
			p->stackPointer++;
			MemoryWrite(p, p->stackPointer, p->accumulatorA);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_PSHB(MC6803E_MPU * p)
		Boolean:	B -> Stack
		Flags:		
*/
void ALU_MC6803E_PSHB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x37: // PSHB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "PSHB");
			p->stackPointer++;
			MemoryWrite(p, p->stackPointer, p->accumulatorB);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_PULA(MC6803E_MPU * p)
		Boolean:	Stack -> A
		Flags:		
*/
void ALU_MC6803E_PULA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x32: // PULA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "PULA");
			p->accumulatorA = MemoryRead(p, p->stackPointer);
			p->stackPointer--;
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_PULB(MC6803E_MPU * p)
		Boolean:	Stack -> B
		Flags:		
*/
void ALU_MC6803E_PULB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x33: // PULB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "PULB");
			p->accumulatorB = MemoryRead(p, p->stackPointer);
			p->stackPointer--;
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_ROL(MC6803E_MPU * p)
		Boolean:	b7 -> Temp; b6 b5 b4 b3 b2 b1 b0 C -> M; Temp -> C
		Flags:		N Z V C
*/
void ALU_MC6803E_ROL(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	uint8_t result, oldBit7;

	switch (instruction) {
		case 0x69: // ROL Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ROL %02X(x)", unsigned_payload);
			oldBit7 = (MemoryReadIndexValue(p, unsigned_payload) & 0x80);
			result = MemoryWriteIndexValue(p, unsigned_payload, (MemoryReadIndexValue(p, unsigned_payload) << 1));
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x79: // ROL Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ROL %04X", unsigned_payload_double);
			oldBit7 = (MemoryRead(p, unsigned_payload_double) & 0x80);
			result = MemoryWrite(p, unsigned_payload_double, (MemoryRead(p, unsigned_payload_double) << 1));
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, oldBit7);
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, result);
}

/*
		void ALU_MC6803E_ROLA(MC6803E_MPU * p)
		Boolean:	b7 -> Temp; b6 b5 b4 b3 b2 b1 b0 C -> A; Temp -> C
		Flags:		N Z V C
*/
void ALU_MC6803E_ROLA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	uint8_t oldBit7 = (p->accumulatorA & 0x80);

	switch (instruction) {
		case 0x49: // ROLA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "ROLA");
			p->accumulatorA = (p->accumulatorA << 1);
			p->accumulatorA = (p->accumulatorA | ALU_MC6803E_GetFlag(p, MC6803E_FLAG_C));
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, oldBit7);
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
}

/*
		void ALU_MC6803E_ROLB(MC6803E_MPU * p)
		Boolean:	b7 -> Temp; b6 b5 b4 b3 b2 b1 b0 C -> B; Temp -> C
		Flags:		N Z V C
*/
void ALU_MC6803E_ROLB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	
	uint8_t oldBit7 = (p->accumulatorB & 0x80);
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, (p->accumulatorB & 0x80));
	
	switch (instruction) {
		case 0x59: // ROLB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "ROLB");
			p->accumulatorA = (p->accumulatorB << 1);
			p->accumulatorA = (p->accumulatorB | ALU_MC6803E_GetFlag(p, MC6803E_FLAG_C));
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_C, oldBit7);
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorB);
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_ROR(MC6803E_MPU * p)
		Boolean:	b0 -> Temp; C b7 b6 b5 b4 b3 b2 b1 b0 -> M; Temp -> C
		Flags:		N Z V C
*/
void ALU_MC6803E_ROR(MC6803E_MPU * p)
{
		uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x66: // ROR Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ROR %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x76: // ROR Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "ROR %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_RORA(MC6803E_MPU * p)
		Boolean:	b0 -> Temp; C b7 b6 b5 b4 b3 b2 b1 b0 -> A; Temp -> C
		Flags:		N Z V C
*/
void ALU_MC6803E_RORA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x46: // RORA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "RORA");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_RORB(MC6803E_MPU * p)
		Boolean:	b0 -> Temp; C b7 b6 b5 b4 b3 b2 b1 b0 -> B; Temp -> C
		Flags:		N Z V C
*/
void ALU_MC6803E_RORB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x56: // RORB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "RORB");
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_SBA(MC6803E_MPU * p)
		Boolean:	A - B -> A
		Flags:		N Z V C
*/
void ALU_MC6803E_SBA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x10: // SBA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "SBA");
			p->accumulatorA = (p->accumulatorA - p->accumulatorB);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_N, (p->accumulatorA & 0x80));
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_SBCA(MC6803E_MPU * p)
		Boolean:	A - M - C -> A
		Flags:		N Z V C
*/
void ALU_MC6803E_SBCA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x82: // SBCA Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SBCA #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x92: // SBCA Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SBCA %02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xA2: // SBCA Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SBCA %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xB2: // SBCA Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SBCA %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_SBCB(MC6803E_MPU * p)
		Boolean:	B - M - C -> B
		Flags:		N Z V C
*/
void ALU_MC6803E_SBCB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0xC2: // SBCB Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SBCB #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xD2: // SBCB Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SBCB %02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xE2: // SBCB Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SBCB %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xF2: // SBCB Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SBCB %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_STAA(MC6803E_MPU * p)
		Boolean:	A -> M
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_STAA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x97: // STAA Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STAA %02X", unsigned_payload);
			MemoryWrite(p, direct_address, p->accumulatorA);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xA7: // STAA Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STAA %02X(x)", unsigned_payload);
			MemoryWriteIndexValue(p, unsigned_payload, p->accumulatorA);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xB7: // STAA Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STAA %04X", unsigned_payload_double);
			MemoryWrite(p, unsigned_payload_double, p->accumulatorA);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, p->accumulatorA);
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_STAB(MC6803E_MPU * p)
		Boolean:	B -> M
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_STAB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0xD7: // STAB Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STAB %02X", unsigned_payload);
			MemoryWrite(p, direct_address, p->accumulatorB);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xE7: // STAB Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STAB %02X(x)", unsigned_payload);
			MemoryWriteIndexValue(p, unsigned_payload, p->accumulatorB);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xF7: // STAB Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STAB %04X", unsigned_payload_double);
			MemoryWrite(p, unsigned_payload_double, p->accumulatorB);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, p->accumulatorB);
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorB);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_STD(MC6803E_MPU * p)
		Boolean:	D -> M:(M + 1)
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_STD(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0xDD: // STD Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STD %02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xED: // STD Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STD %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xFD: // STD Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "STD %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_SUBA(MC6803E_MPU * p)
		Boolean:	A - M -> A
		Flags:		N Z V C
*/
void ALU_MC6803E_SUBA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x80: // SUBA Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SUBA #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x90: // SUBA Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SUBA %02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xA0: // SUBA Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SUBA %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xB0: // SUBA Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SUBA %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_SUBB(MC6803E_MPU * p)
		Boolean:	B - M -> B
		Flags:		N Z V C
*/
void ALU_MC6803E_SUBB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0xC0: // SUBB Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SUBB #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xD0: // SUBB Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SUBB %02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xE0: // SUBB Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SUBB %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xF0: // SUBB Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SUBB %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_SUBD(MC6803E_MPU * p)
		Boolean:	D - M:(M + 1) -> D
		Flags:		N Z V C
*/
void ALU_MC6803E_SUBD(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x83: // SUBD Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SUBD #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x93: // SUBD Direct
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SUBD %02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xA3: // SUBD Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SUBD %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xB3: // SUBD Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "SUBD %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_TAB(MC6803E_MPU * p)
		Boolean:	A -> B
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_TAB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x16: // TAB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "TAB");
			p->accumulatorB = p->accumulatorA;
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorB);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorB & 0x80));
}

/*
		void ALU_MC6803E_TBA(MC6803E_MPU * p)
		Boolean:	B -> A
		Flags:		N Z (0 -> V)
*/
void ALU_MC6803E_TBA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x17: // TBA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "TBA");
			p->accumulatorA = p->accumulatorB;
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->accumulatorA);
	ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->accumulatorA & 0x80));
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_TST(MC6803E_MPU * p)
		Boolean:	M - 00
		Flags:		N Z (0 -> V) (0 -> C)
*/
void ALU_MC6803E_TST(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x6D: // TST Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "TST %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x7D: // TST Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "TST %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_TSTA(MC6803E_MPU * p)
		Boolean:	A - 00
		Flags:		N Z (0 -> V) (0 -> C)
*/
void ALU_MC6803E_TSTA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x4D: // TSTA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "TSTA");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_TSTB(MC6803E_MPU * p)
		Boolean:	B - 00
		Flags:		N Z (0 -> V) (0 -> C)
*/
void ALU_MC6803E_TSTB(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x5D: // TSTB Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "TSTB");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BRA(MC6803E_MPU * p)
		Branch Test:	None
*/
void ALU_MC6803E_BRA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;
	
	switch (instruction) {
		case 0x20: // BRA Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BRA #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BRN(MC6803E_MPU * p)
		Branch Test:	None
*/
void ALU_MC6803E_BRN(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x21: // BRN Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BRN #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BCC(MC6803E_MPU * p)
		Branch Test:	C = 0
*/
void ALU_MC6803E_BCC(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x24: // BCC Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BCC #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BCS(MC6803E_MPU * p)
		Branch Test:	C = 1
*/
void ALU_MC6803E_BCS(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x25: // BCS Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BCS #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BLO(MC6803E_MPU * p)
		Branch Test:	Z = 1
*/
void ALU_MC6803E_BLO(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x25: // BLO Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BLO #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BEQ(MC6803E_MPU * p)
		Branch Test:	Z = 0
*/
void ALU_MC6803E_BEQ(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x27: // BEQ Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BEQ #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BGE(MC6803E_MPU * p)
		Branch Test:	N ^ V = 0
*/
void ALU_MC6803E_BGE(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x2C: // BGE Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BGE #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BGT(MC6803E_MPU * p)
		Branch Test:	Z | (N ^ V) = 1
*/
void ALU_MC6803E_BGT(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x2E: // BGT Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BGT #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BHI(MC6803E_MPU * p)
		Branch Test:	C + Z = 0
*/
void ALU_MC6803E_BHI(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x22: // BHI Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BHI #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BHS(MC6803E_MPU * p)
		Branch Test:	C = 0
*/
void ALU_MC6803E_BHS(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x24: // BHS Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BHS #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BLE(MC6803E_MPU * p)
		Branch Test:	Z + (N^V) = 1
*/
void ALU_MC6803E_BLE(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x2F: // BLE Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BLE #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BLS(MC6803E_MPU * p)
		Branch Test:	C + Z = 1
*/
void ALU_MC6803E_BLS(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x23: // BLS Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BLS #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BLT(MC6803E_MPU * p)
		Branch Test:	N ^ V = 1
*/
void ALU_MC6803E_BLT(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x2D: // BLT Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BLT #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BMI(MC6803E_MPU * p)
		Branch Test:	N = 1
*/
void ALU_MC6803E_BMI(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x2B: // BMI Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BMI #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BNE(MC6803E_MPU * p)
		Branch Test:	Z = 0
*/
void ALU_MC6803E_BNE(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x26: // BNE Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BNE #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BVC(MC6803E_MPU * p)
		Branch Test:	V = 0
*/
void ALU_MC6803E_BVC(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x28: // BVC Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BVC #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BVS(MC6803E_MPU * p)
		Branch Test:	V = 1
*/
void ALU_MC6803E_BVS(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x29: // BVS Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BVS #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BPL(MC6803E_MPU * p)
		Branch Test:	N = 0
*/
void ALU_MC6803E_BPL(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x2A: // BPL Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BPL #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_BSR(MC6803E_MPU * p)
		Boolean:	
*/
void ALU_MC6803E_BSR(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x8D: // BSR Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "BSR #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_JMP(MC6803E_MPU * p)
		Boolean:	
		Flags:		
*/
void ALU_MC6803E_JMP(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));

	switch (instruction) {
		case 0x6E: // JMP Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "JMP %02X(x)", unsigned_payload);
			p->pc = (MemoryReadIndexValue(p, unsigned_payload) - 2);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0x7E: // JMP Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "JMP %04X", unsigned_payload_double);
			p->pc = (unsigned_payload_double - 3);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_JSR(MC6803E_MPU * p)
		Boolean:	
		Flags:		
*/
void ALU_MC6803E_JSR(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x9D: // JSR Immediate
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "JSR #$%02X", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xAD: // JSR Index
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "JSR %02X(x)", unsigned_payload);
			ALU_MC6803E_IncrementPC(p, 1);
			break;
		case 0xBD: // JSR Extended
			ALU_MC6803E_SetCurrentMneunomicWithPayload(p, "JSR %04X", unsigned_payload_double);
			ALU_MC6803E_IncrementPC(p, 2);
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_RTI(MC6803E_MPU * p)
		Boolean:	
		Flags:		H I N Z V C
*/
void ALU_MC6803E_RTI(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x3B: // RTI Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "RTI");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_RTS(MC6803E_MPU * p)
		Boolean:	
		Flags:		
*/
void ALU_MC6803E_RTS(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x39: // RTS Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "RTS");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_SWI(MC6803E_MPU * p)
		Boolean:	
		Flags:		(1 -> I)
*/
void ALU_MC6803E_SWI(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x3F: // SWI Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "SWI");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_WAI(MC6803E_MPU * p)
		Boolean:	
		Flags:		
*/
void ALU_MC6803E_WAI(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	switch (instruction) {
		case 0x3E: // WAI Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "WAI");
			break;
		default:
			break;
	}
}

// NOT IMPLEMENTED
/*
		void ALU_MC6803E_TESTPC(MC6803E_MPU * p)
		Boolean:	
		Flags:		H I N Z V C
*/
void ALU_MC6803E_TESTPC(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);
	uint8_t unsigned_payload = (uint8_t)MemoryRead(p, (p->pc+1));
	int8_t signed_payload = (int8_t)MemoryRead(p, (p->pc+1));
	uint16_t unsigned_payload_double = uint16_From_uint8s(MemoryRead(p, (p->pc+1)), MemoryRead(p, (p->pc+2)));
	uint16_t direct_address = (uint16_t)unsigned_payload;

	// @CC what is this?!

	switch (instruction) {
		case 0x4E: // TESTPC Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "TESTPC");
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_CLC(MC6803E_MPU * p)
		Boolean:	0 -> C
		Flags:		(0 -> C)
*/
void ALU_MC6803E_CLC(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x0C: // CLC Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "CLC");
			ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_C);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_CLI(MC6803E_MPU * p)
		Boolean:	0 -> I
		Flags:		(0 -> I)
*/
void ALU_MC6803E_CLI(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x0E: // CLI Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "CLI");
			ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_I);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_CLV(MC6803E_MPU * p)
		Boolean:	0 -> V
		Flags:		(0 -> V)
*/
void ALU_MC6803E_CLV(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x0A: // CLV Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "CLV");
			ALU_MC6803E_UnsetFlag(p, MC6803E_FLAG_V);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_SEC(MC6803E_MPU * p)
		Boolean:	1 -> C
		Flags:		(1 -> C)
*/
void ALU_MC6803E_SEC(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x0D: // SEC Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "SEC");
			ALU_MC6803E_SetFlag(p, MC6803E_FLAG_C);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_SEI(MC6803E_MPU * p)
		Boolean:	1 -> I
		Flags:		(1 -> I)
*/
void ALU_MC6803E_SEI(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x0F: // SEI Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "SEI");
			ALU_MC6803E_SetFlag(p, MC6803E_FLAG_I);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_SEV(MC6803E_MPU * p)
		Boolean:	1 -> V
		Flags:		(1 -> V)
*/
void ALU_MC6803E_SEV(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x0B: // SEV Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "SEV");
			ALU_MC6803E_SetFlag(p, MC6803E_FLAG_V);
			break;
		default:
			break;
	}
}

/*
		void ALU_MC6803E_TAP(MC6803E_MPU * p)
		Boolean:	A -> CCR
		Flags:		H I N Z V C
*/
void ALU_MC6803E_TAP(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x06: // TAP Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "TAP");
			p->flagRegister = p->accumulatorA;
			break;
		default:
			break;
	}
	
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_H, (p->flagRegister & 0x10));
	// @CC why set I here.
	ALU_MC6803E_SetFlagIfNonZero(p, MC6803E_FLAG_N, (p->flagRegister & 0x80));
	ALU_MC6803E_SetFlagIfZero(p, MC6803E_FLAG_Z, p->flagRegister);
	// @CC why set V and C...
}

/*
		void ALU_MC6803E_TPA(MC6803E_MPU * p)
		Boolean:	CCR -> A
		Flags:		
*/
void ALU_MC6803E_TPA(MC6803E_MPU * p)
{
	uint8_t instruction = (uint8_t)MemoryRead(p, p->pc);

	switch (instruction) {
		case 0x07: // TPA Inherent
			ALU_MC6803E_SetCurrentMneunomic(p, "TPA");
			p->accumulatorA = p->flagRegister;
			break;
		default:
			break;
	}
}
