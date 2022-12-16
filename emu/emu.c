/*

8080 Emulator

TODO:
	flesh out algorithm for emulation
		fetch
		decode   ] these two are done in func described above 
		execute  ]
*/


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "disasm.h"

typedef struct flag_set {
	uint8_t zero;
	uint8_t sign;
	uint8_t parity;
	uint8_t carry;
	uint8_t a_carry;
	uint8_t pad;
} flag_set;

typedef struct emu_state {
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;
	uint16_t sp;
	uint16_t pc;
	uint8_t* memory;
	struct flag_set flags;
	uint8_t int_enable;	
} emu_state_t;

emu_state_t* new_state(void);
void delete_state(emu_state_t*);

void unrecognized_instruction(emu_state_t* state);
void emulate_op(emu_state_t* state);
uint16_t join_regpair(uint8_t reg_1, uint8_t reg_2);
bool parity_8(uint8_t n);
void set_flags_logic(uint8_t value, emu_state_t* state);
void set_flags_arithmetic(uint16_t value, emu_state_t* state);

void INR_register(uint8_t* reg, emu_state_t* state);
void DCR_register(uint8_t* reg, emu_state_t* state);
void MVI_register(uint8_t* reg, uint8_t data, emu_state_t* state);
void INX_regpair(uint8_t* reg_1, uint8_t* reg_2);
void DAD_regpair(uint8_t reg_1, uint8_t reg_2, emu_state_t* state);
void DCX_regpair(uint8_t* reg_1, uint8_t* reg_2);
void MOV(uint8_t* to, uint8_t from);
void ADD(uint8_t reg, emu_state_t* state);
void ADC(uint8_t reg, emu_state_t* state);
void SUB(uint8_t reg, emu_state_t* state);
void SBB(uint8_t reg, emu_state_t* state);
void ANA(uint8_t reg, emu_state_t* state);
void XRA(uint8_t reg, emu_state_t* state);
void ORA(uint8_t reg, emu_state_t* state);
void CMP(uint8_t reg, emu_state_t* state);
void RET(emu_state_t* state);
void POP(uint8_t* reg_1, uint8_t* reg_2, emu_state_t* state);
void PUSH(uint8_t reg_1, uint8_t reg_2, emu_state_t* state);
void CALL(uint16_t address, emu_state_t* state);



int main(const int argc, char** argv)
{

	emu_state_t* state = new_state();
	/* ... */

	delete_state(state);
	return 0;
}









emu_state_t* new_state(void)
{
	emu_state_t* state = malloc(sizeof(emu_state_t));
	if (state == NULL) {
		fprintf(stderr, "error: unable to create state\n");
		return NULL;
	}
	state->memory = malloc(0x10000);
	if (state->memory == NULL) {
		fprintf(stderr, "error: unable to allocate state memory\n");
		return NULL;
	}
	return state;
}

void delete_state(emu_state_t* state)
{
	if (state == NULL) {
		return;
	}
	free(state->memory);
	free(state);
}


void unrecognized_instruction(emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer\n");
		return;
	}
	printf("instruction 0x%02x not recognized\n", state->memory[state->pc]);
	state->pc -= 1;
	exit(1);
}

void emulate_op(emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer\n");
		exit(1);
	}
	unsigned char* instruction = &state->memory[state->pc];
	unsigned char opcode = *instruction;
	
	uint32_t temp_32;
	uint16_t temp_16;
	uint8_t  temp_8;
	bool     temp_1;

	// So we can see which instruction is decoded:
	disasm_op_8080(state->memory, state->pc, false);

	switch (opcode)
	{
		case 0x00: // NOP
			break;
		case 0x01: // LXI into BC
			state->c = instruction[1];
			state->b = instruction[2];
			state->pc += 2;
			break;
		case 0x02: // STAX B (store A into memory at location (BC))
			state->memory[join_regpair(state->b, state->c)] = state->a;
			break;
		case 0x03: // INX B // BC <- BC + 1
			// temp_16 = join_regpair(state->b, state->c) + 1;
			// state->b = temp_16 >> 8;
			// state->c = temp_16 & 0xff;
			// // need to determine if this should update flags
			INX_regpair(&(state->b), &(state->c));
			
			break;
		case 0x04: // INR B // B <- B + 1
			// temp_8 = state->b + 1;
			// state->flags.zero = (temp_8 == 0); // check if num = 0
			// state->flags.sign = (temp_8 & 0x80); // check for MSB being set
			// state->flags.carry = (state->b == 0xff); // only works for this example really
			// state->flags.parity = parity_8(temp_8);
			// state->b = temp_8;
			INR_register(&(state->b), state);
			break;
		case 0x05: // DCR B // B <- B - 1 
			// temp_8 = state->b - 1;
			// state->flags.zero = (temp_8 == 0);
			// state->flags.sign = (temp_8 & 0x80);
			// state->flags.carry = (state->b == 0); // specific to this instr
			// state->flags.parity = parity_8(temp_8);
			// state->b = temp_8;
			DCR_register(&(state->b), state);
			break;
		case 0x06: // MVI B,D8
			MVI_register(&(state->b), instruction[1], state);
			// state->b = instruction[1];
			// state->pc++;
			break;
		case 0x07: // RLC
			temp_1 = state->a >> 7;
			state->a = (state->a << 1) | temp_1;
			state->flags.carry = temp_1;
			break;
		case 0x08: break; // NOP
		case 0x09: // DAD B // HL <- HL + BC
			// temp_32 = join_regpair(state->h, state->l) + join_regpair(state->b, state->c);
			// state->flags.carry = (temp_32 & 0xffff0000) > 0;
			// // now extract H,L
			// state->h = temp_32 >> 8;
			// state->l = temp_32 & 0xff;
			DAD_regpair(state->b, state->c, state);
			break;
		case 0x0a: // LDAX B // A <- (BC)
			state->a = state->memory[join_regpair(state->b, state->c)];
			break;
		case 0x0b: // DCX B // BC <- BC - 1
			// temp_16 = join_regpair(state->b, state->c) - 1;
			// state->b = temp_16 >> 8;
			// state->c = temp_16 & 0xff;
			DCX_regpair(&(state->b), &(state->c));
			break;
		case 0x0c: // INR C 
			INR_register(&(state->c), state);
			break;
		case 0x0d: // DCR C
			DCR_register(&(state->c), state);
			break;
		case 0x0e: // MVI C,D8
			MVI_register(&(state->c), instruction[1], state);
			break;
		case 0x0f: // RRC
			temp_8 = state->a & 1;
			state->a = (state->a >> 1) | (temp_8 << 7); // double check that this is right
			state->flags.carry = temp_8;
			break;
		case 0x10: break;
		case 0x11: // LXI D,D16
			state->d = instruction[2];
			state->e = instruction[1];
			state->pc += 2;
			break;
		case 0x12: // STAX D
			state->memory[join_regpair(state->d, state->e)] = state->a;
			break;
		case 0x13: // INX D
			INX_regpair(&(state->d), &(state->e));
			break;
		case 0x14: // INR D
			INR_register(&(state->d), state);
			break;
		case 0x15: // DCR D
			DCR_register(&(state->d), state);
			break;
		case 0x16: // MVI D,D8
			MVI_register(&(state->d), instruction[1], state);
			break;
		case 0x17: // RAL
			temp_8 = state->a >> 7;
			state->a <<= 1;
			state->a |= (state->flags.carry & 1);
			state->flags.carry = temp_8;
			break;
		case 0x18: break;
		case 0x19: // DAD D
			DAD_regpair(state->d, state->e, state);
			break;
		case 0x1a: // LDAX D
			state->a = state->memory[join_regpair(state->d, state->e)];
			break;
		case 0x1b: // DCX D
			DCX_regpair(&(state->d), &(state->e));
			break;
		case 0x1c: // INR E
			INR_register(&(state->e), state);
			break;
		case 0x1d: // DCR E
			DCR_register(&(state->e), state);
			break;
		case 0x1e: // MVI E,D8  
			MVI_register(&(state->e), instruction[1], state);
			break;
		case 0x1f: // RAR // TODO - make sure this is right
			temp_8 = state->a & 1;
			state->a = (state->a >> 1) | ((state->flags.carry & 1) << 7);
			state->flags.carry = temp_8;
			break;
		case 0x20: break;
		case 0x21: // LXI H,D16
			state->h = instruction[2];
			state->l = instruction[1];
			state->pc += 2;
			break;
		case 0x22: // SHLD adr
			// I think the addr is stored in little endian in the instr, not 100% sure though
			temp_16 = (instruction[2] << 8) | instruction[1];
			state->memory[temp_16] = state->l;
			state->memory[temp_16 + 1] = state->h;
			state->pc += 2;
			break;
		case 0x23: // INX H
			INX_regpair(&(state->h), &(state->l));
			break;
		case 0x24: // INR H
			INR_register(&(state->h), state);
			break;
		case 0x25: // DCR H
			DCR_register(&(state->h), state);
			break;
		case 0x26: // MVI H,D8
			MVI_register(&(state->h), instruction[1], state);
			break;
		case 0x27: // DAA
			// not implemented yet
			unrecognized_instruction(state);
			break;
		case 0x28: break;
		case 0x29: // DAD H
			DAD_regpair(state->h, state->l, state);
			break;
		case 0x2a: // LHLD adr
			temp_16 = (instruction[2] << 8) | instruction[1];
			state->l = state->memory[temp_16];
			state->h = state->memory[temp_16 + 1];
			state->pc += 2;
			break;
		case 0x2b: // DCX H
			DCX_regpair(&(state->h), &(state->l));
			break;
		case 0x2c: // INR L
			INR_register(&(state->l), state);
			break;
		case 0x2d: // DCR L
			DCR_register(&(state->l), state);
			break;
		case 0x2e: // MVI L,D8
			MVI_register(&(state->l), instruction[1], state);
			break;
		case 0x2f: // CMA
			state->a = ~(state->a);
			break;
		case 0x30: break;
		case 0x31: // LXI SP,D16
			state->sp = (instruction[2] << 8) | instruction[1];
			state->pc += 2;
			break;
		case 0x32: // STA adr
			temp_16 = (instruction[2] << 8) | instruction[1];
			state->memory[temp_16] = state->a;
			state->pc += 2;
			break;
		case 0x33: // INX SP
			state->sp += 1;
			break;
		case 0x34: // INR M
			// TODO - make sure this works
			INR_register(&(state->memory[join_regpair(state->h, state->l)]), state);
			break;
		case 0x35: // DCR M
			// TODO - make sure this works
			DCR_register(&(state->memory[join_regpair(state->h, state->l)]), state);
			break;
		case 0x36: // MVI M,D8
			// TODO - make sure this works
			MVI_register(&(state->memory[join_regpair(state->h, state->l)]), instruction[1], state);
			break;
		case 0x37: // STC
			state->flags.carry = 1;
			break;
		case 0x38: break;
		case 0x39: // DAD SP
			temp_32 = join_regpair(state->h, state->l) + join_regpair((state->sp & 0xff00) >> 8, state->sp & 0xff);
			state->flags.carry = (temp_32 & 0xffff0000) > 0;
			// now extract H,L
			state->h = (temp_32 & 0xff00) >> 8;
			state->l = temp_32 & 0xff;
			break;
		case 0x3a: // LDA adr
			state->a = state->memory[(instruction[2] << 8) | instruction[1]];
			state->pc += 2;
			break;
		case 0x3b: // DCX SP
			state->sp -= 1;
			break;
		case 0x3c: // INR A
			INR_register(&(state->a), state);
			break;
		case 0x3d: // DCR A
			DCR_register(&(state->a), state);
			break;
		case 0x3e: // MVI A,D8
			MVI_register(&(state->a), instruction[1], state);
			break;
		case 0x3f: // CMC
			state->flags.carry = !state->flags.carry;
			break;
		case 0x40: // MOV B,B
			MOV(&(state->b), state->b);
			break;
		case 0x41: // MOV B,C
			MOV(&(state->b), state->c);
			break;
		case 0x42: // MOV B,D
			MOV(&(state->b), state->d);
			break;
		case 0x43: // MOV B,E
			MOV(&(state->b), state->e);
			break;
		case 0x44: // MOV B,H
			MOV(&(state->b), state->h);
			break;
		case 0x45: // MOV B,L
			MOV(&(state->b), state->l);
			break;
		case 0x46: // MOV B,M
			MOV(&(state->b), state->memory[join_regpair(state->h, state->l)]);
			break;
		case 0x47: // MOV B,A
			MOV(&(state->b), state->a);
			break;
		case 0x48: // MOV C,B
			MOV(&(state->c), state->b);
			break;
		case 0x49: // MOV C,C
			MOV(&(state->c), state->c);
			break;
		case 0x4a: // MOV C,D
			MOV(&(state->c), state->d);
			break;
		case 0x4b: // MOV C,E
			MOV(&(state->c), state->e);
			break;
		case 0x4c: // MOV C,H
			MOV(&(state->c), state->h);
			break;
		case 0x4d: // MOV C,L
			MOV(&(state->c), state->l);
			break;
		case 0x4e: // MOV C,M
			MOV(&(state->c), state->memory[join_regpair(state->h, state->l)]);
			break;
		case 0x4f: // MOV C,A
			MOV(&(state->c), state->a);
			break;
		case 0x50: // MOV D,B
			MOV(&(state->d), state->b);
			break;
		case 0x51: // MOV D,C
			MOV(&(state->d), state->c);
			break;
		case 0x52: // MOV D,D
			MOV(&(state->d), state->d);
			break;
		case 0x53: // MOV D,E
			MOV(&(state->d), state->e);
			break;
		case 0x54: // MOV D,H
			MOV(&(state->d), state->h);
			break;
		case 0x55: // MOV D,L
			MOV(&(state->d), state->l);
			break;
		case 0x56: // MOV D,M
			MOV(&(state->d), state->memory[join_regpair(state->h, state->l)]);
			break;
		case 0x57: // MOV D,A
			MOV(&(state->d), state->a);
			break;
		case 0x58: // MOV E,B
			MOV(&(state->e), state->b);
			break;
		case 0x59: // MOV E,C
			MOV(&(state->e), state->c);
			break;
		case 0x5a: // MOV E,D
			MOV(&(state->e), state->d);
			break;
		case 0x5b: // MOV E,E
			MOV(&(state->e), state->e);
			break;
		case 0x5c: // MOV E,H
			MOV(&(state->e), state->h);
			break;
		case 0x5d: // MOV E,L
			MOV(&(state->e), state->l);
			break;
		case 0x5e: // MOV E,M
			MOV(&(state->e), state->memory[join_regpair(state->h, state->l)]);
			break;
		case 0x5f: // MOV E,A
			MOV(&(state->e), state->a);
			break;
		case 0x60: // MOV H,B
			MOV(&(state->h), state->b);
			break;
		case 0x61: // MOV H,C
			MOV(&(state->h), state->c);
			break;
		case 0x62: // MOV H,D
			MOV(&(state->h), state->d);
			break;
		case 0x63: // MOV H,E
			MOV(&(state->h), state->e);
			break;
		case 0x64: // MOV H,H
			MOV(&(state->h), state->h);
			break;
		case 0x65: // MOV H,L
			MOV(&(state->h), state->l);
			break;
		case 0x66: // MOV H,M
			MOV(&(state->h), state->memory[join_regpair(state->h, state->l)]);
			break;
		case 0x67: // MOV H,A
			MOV(&(state->h), state->a);
			break;
		case 0x68: // MOV L,B
			MOV(&(state->l), state->b);
			break;
		case 0x69: // MOV L,C
			MOV(&(state->l), state->c);
			break;
		case 0x6a: // MOV L,D
			MOV(&(state->l), state->d);
			break;
		case 0x6b: // MOV L,E
			MOV(&(state->l), state->e);
			break;
		case 0x6c: // MOV L,H
			MOV(&(state->l), state->h);
			break;
		case 0x6d: // MOV L,L
			MOV(&(state->l), state->l);
			break;
		case 0x6e: // MOV L,M
			MOV(&(state->l), state->memory[join_regpair(state->h, state->l)]);
			break;
		case 0x6f: // MOV L,A
			MOV(&(state->l), state->a);
			break;
		case 0x70: // MOV M,B
			MOV(&(state->memory[join_regpair(state->h, state->l)]), state->b);
			break;
		case 0x71: // MOV M,C
			MOV(&(state->memory[join_regpair(state->h, state->l)]), state->c);
			break;
		case 0x72: // MOV M,D
			MOV(&(state->memory[join_regpair(state->h, state->l)]), state->d);
			break;
		case 0x73: // MOV M,E
			MOV(&(state->memory[join_regpair(state->h, state->l)]), state->e);
			break;
		case 0x74: // MOV M,H
			MOV(&(state->memory[join_regpair(state->h, state->l)]), state->h);
			break;
		case 0x75: // MOV M,L
			MOV(&(state->memory[join_regpair(state->h, state->l)]), state->l);
			break;
		case 0x76: // MOV M,M
			MOV(&(state->memory[join_regpair(state->h, state->l)]), state->memory[join_regpair(state->h, state->l)]);
			break;
		case 0x77: // MOV M,A
			MOV(&(state->memory[join_regpair(state->h, state->l)]), state->a);
			break;
		case 0x78: // MOV A,B
			MOV(&(state->a), state->b);
			break;
		case 0x79: // MOV A,C
			MOV(&(state->a), state->c);
			break;
		case 0x7a: // MOV A,D
			MOV(&(state->a), state->d);
			break;
		case 0x7b: // MOV A,E
			MOV(&(state->a), state->e);
			break;
		case 0x7c: // MOV A,H
			MOV(&(state->a), state->h);
			break;
		case 0x7d: // MOV A,L
			MOV(&(state->a), state->l);
			break;
		case 0x7e: // MOV A,M
			MOV(&(state->a), state->memory[join_regpair(state->h, state->l)]);
			break;
		case 0x7f: // MOV A,A
			MOV(&(state->a), state->a);
			break;
		case 0x80: // ADD B
			ADD(state->b, state);
			break;
		case 0x81: // ADD C
			ADD(state->c, state);
			break;
		case 0x82: // ADD D
			ADD(state->d, state);
			break;
		case 0x83: // ADD E
			ADD(state->e, state);
			break;
		case 0x84: // ADD H
			ADD(state->h, state);
			break;
		case 0x85: // ADD L
			ADD(state->l, state);
			break;
		case 0x86: // ADD M
			ADD(state->memory[join_regpair(state->h, state->l)], state);
			break;
		case 0x87: // ADD A
			ADD(state->a, state);
			break;
		case 0x88: // ADC B
			ADC(state->b, state);
			break;
		case 0x89: // ADC C
			ADC(state->c, state);
			break;
		case 0x8a: // ADC D
			ADC(state->d, state);
			break;
		case 0x8b: // ADC E
			ADC(state->e, state);
			break;
		case 0x8c: // ADC H
			ADC(state->h, state);
			break;
		case 0x8d: // ADC L
			ADC(state->l, state);
			break;
		case 0x8e: // ADC M
			ADC(state->memory[join_regpair(state->h, state->l)], state);
			break;
		case 0x8f: // ADC A
			ADC(state->a, state);
			break;
		case 0x90: // SUB B
			SUB(state->b, state);
			break;
		case 0x91: // SUB C
			SUB(state->c, state);
			break;
		case 0x92: // SUB D
			SUB(state->d, state);
			break;
		case 0x93: // SUB E
			SUB(state->e, state);
			break;
		case 0x94: // SUB H
			SUB(state->h, state);
			break;
		case 0x95: // SUB L
			SUB(state->l, state);
			break;
		case 0x96: // SUB M
			SUB(state->memory[join_regpair(state->h, state->l)], state);
			break;
		case 0x97: // SUB A
			SUB(state->a, state);
			break;
		case 0x98: // SBB B
			SBB(state->b, state);
			break;
		case 0x99: // SBB C
			SBB(state->c, state);
			break;
		case 0x9a: // SBB D
			SBB(state->d, state);
			break;
		case 0x9b: // SBB E
			SBB(state->e, state);
			break;
		case 0x9c: // SBB H
			SBB(state->h, state);
			break;
		case 0x9d: // SBB L
			SBB(state->l, state);
			break;
		case 0x9e: // SBB M
			SBB(state->memory[join_regpair(state->h, state->l)], state);
			break;
		case 0x9f: // SBB A
			SBB(state->a, state);
			break;
		case 0xa0: // ANA B
			ANA(state->b, state);
			break; 
		case 0xa1: // ANA C
			ANA(state->c, state);
			break;
		case 0xa2: // ANA D
			ANA(state->d, state);
			break;
		case 0xa3: // ANA E
			ANA(state->e, state);
			break;
		case 0xa4: // ANA H
			ANA(state->h, state);
			break;
		case 0xa5: // ANA L
			ANA(state->l, state);
			break;
		case 0xa6: // ANA M
			ANA(state->memory[join_regpair(state->h, state->l)], state);
			break;
		case 0xa7: // ANA A
			ANA(state->a, state);
			break;
		case 0xa8: // XRA B
			XRA(state->b, state);
			break;
		case 0xa9: // XRA C
			XRA(state->c, state);
			break;
		case 0xaa: // XRA D
			XRA(state->d, state);
			break;
		case 0xab: // XRA E
			XRA(state->e, state);
			break;
		case 0xac: // XRA H
			XRA(state->h, state);
			break;
		case 0xad: // XRA L
			XRA(state->l, state);
			break;
		case 0xae: // XRA M
			XRA(state->memory[join_regpair(state->h, state->l)], state);
			break;
		case 0xaf: // XRA A
			XRA(state->a, state);
			break;
		case 0xb0: // ORA B
			ORA(state->b, state);
			break;
		case 0xb1: // ORA C
			ORA(state->c, state);
			break;
		case 0xb2: // ORA D
			ORA(state->d, state);
			break;
		case 0xb3: // ORA E
			ORA(state->e, state);
			break;
		case 0xb4: // ORA H
			ORA(state->h, state);
			break;
		case 0xb5: // ORA L
			ORA(state->l, state);
			break;
		case 0xb6: // ORA M
			ORA(state->memory[join_regpair(state->h, state->l)], state);
			break;
		case 0xb7: // ORA A
			ORA(state->a, state);
			break;
		case 0xb8: // CMP B
			CMP(state->b, state);
			break;
		case 0xb9: // CMP C
			CMP(state->c, state);
			break;
		case 0xba: // CMP D
			CMP(state->d, state);
			break;
		case 0xbb: // CMP E
			CMP(state->e, state);
			break;
		case 0xbc: // CMP H
			CMP(state->h, state);
			break;
		case 0xbd: // CMP L
			CMP(state->l, state);
			break;
		case 0xbe: // CMP M
			CMP(state->memory[join_regpair(state->h, state->l)], state);
			break;
		case 0xbf: // CMP A 
			CMP(state->a, state);
			break;
		case 0xc0: // RNZ
			if (state->flags.zero == 0)
			{
				RET(state);
			}
			break;
		case 0xc1: // POP B
			// state->c = state->memory[state->sp];
			// state->b = state->memory[state->sp + 1];
			// state->sp += 2;
			POP(&(state->c), &(state->b), state);
			break;
		case 0xc2: // JNZ adr
			if (state->flags.zero == 0)
			{
				state->pc = (instruction[2] << 8) | instruction[1];
			} else {
				state->pc += 2;
			}
			break;
		case 0xc3: // JMP adr
			state->pc = (instruction[2] << 8) | instruction[1];
			break;
		case 0xc4: // CNZ adr
			if (state->flags.zero == 0) {
				CALL((instruction[2] << 8) | instruction[1], state);
			} else {
				state->pc += 2;
			}
			break;
		case 0xc5: // PUSH B
			PUSH(state->c, state->b, state);
			break;
		case 0xc6: // ADI D8
			ADD(instruction[1], state);
			state->pc++;
			break;
		case 0xc7: // RST 0
			CALL(0x0, state);
			break;
		case 0xc8: // RZ
			if (state->flags.zero) {
				RET(state);
			}
		case 0xc9: // RET
			RET(state);
			break;
		case 0xca: // JZ adr
			if (state->flags.zero) {
				state->pc = (instruction[2] << 8) | instruction[1];
			} else {
				state->pc += 2;
			}
			break;
		case 0xcb: break;
		case 0xcc: // CZ adr
			if (state->flags.zero) {
				CALL((instruction[2] << 8) | instruction[1], state);
			} else {
				state->pc += 2;
			}
			break;
		case 0xcd: // CALL adr
			CALL((instruction[2] << 8) | instruction[1], state);
			break;
		case 0xce: // ACI D8
			ADC(instruction[1], state);
			state->pc++;
			break;
		case 0xcf: // RST 1
			CALL(0x8, state);
			break;
		case 0xd0: // RNC
			if (state->flags.carry == 0) {
				RET(state);
			} 
			break;
		case 0xd1: // POP D
			POP(&(state->e), &(state->d), state);
			break;
		case 0xd2: // JNC adr
			if (state->flags.carry == 0) {
				state->pc = (instruction[2] << 8) | instruction[1];
			} else {
				state->pc += 2;
			}
		case 0xd3: // OUT D8
			// TODO
			state->pc++;
			break;
		case 0xd4: // CNC adr
			if (state->flags.carry == 0) {
				CALL((instruction[2] << 8) | instruction[1], state);
			} else {
				state->pc += 2;
			}
			break;
		case 0xd5: // PUSH D
			PUSH(state->e, state->d, state);
			break;
		case 0xd6: // SUI D8
			SUB(instruction[1], state);
			state->pc++;
			break;
		case 0xd7: // RST 2
			CALL(0x10, state);
			break;
		case 0xd8: // RC
			if (state->flags.carry) {
				RET(state);
			}
			break;
		case 0xd9: break;
		case 0xda: // JC adr
			if (state->flags.carry) {
				state->pc = (instruction[2] << 8) | instruction[1];
			} else {
				state->pc += 2;
			}
			break;
		case 0xdb: // IN D8

			// TODO
			state->pc++;
			break;
		case 0xdc: // CC adr
			if (state->flags.carry) {
				CALL((instruction[2] << 8) | instruction[1], state);
			} else {
				state->pc += 2;
			}
			break;
		case 0xdd: break;
		case 0xde: // SBI D8
			SBB(instruction[1], state);
			state->pc += 1;
			break;
		case 0xdf: // RST 3
			CALL(0x18, state);
			break;
		case 0xe0: // RPO 
			if (state->flags.parity == 0) {
				RET(state);
			}
			break;
		case 0xe1: // POP H
			POP(&(state->l), &(state->h), state);
			break;
		case 0xe2: // JPO adr
			if (state->flags.parity == 0) {
				state->pc = (instruction[2] << 8) | instruction[1];
			} else {
				state->pc += 2;
			}
			break;
		case 0xe3: // XTHL
			temp_8 = state->l;
			state->l = state->memory[state->sp];
			state->memory[state->sp] = temp_8;
			temp_8 = state->h;
			state->h = state->memory[state->sp + 1];
			state->memory[state->sp + 1] = temp_8;
			break;
		case 0xe4: // CPO adr
			if (state->flags.parity == 0) {
				CALL((instruction[2] << 8) | instruction[1], state);
			} else {
				state->sp += 2;
			}
			break;
		case 0xe5: // PUSH H
			PUSH(state->l, state->h, state);
			break;
		case 0xe6: // ANI D8
			ANA(instruction[1], state);
			state->sp++;
			break;
		case 0xe7: // RST 4
			CALL(0x20, state);
			break;
		case 0xe8: // RPE
			if (state->flags.parity) {
				RET(state);
			}
			break;
		case 0xe9: // PCHL
			state->pc = (state->h << 8) | state->l;
			break;
		case 0xea: // JPE adr
			if (state->flags.parity) {
				state->pc = (instruction[2] << 8) | instruction[1];
			} else {
				state->pc += 2;
			}
			break;
		case 0xeb: // XCHG
			temp_8 = state->h;
			state->h = state->d;
			state->d = temp_8;
			temp_8 = state->l;
			state->l = state->e;
			state->e = temp_8;
			break;
		case 0xec: // CPE adr
			if (state->flags.parity) {
				CALL((instruction[2] << 8) | instruction[1], state);
			} else {
				state->pc += 2;
			}
			break;
		case 0xed: break;
		case 0xee: // XRI D8
			XRA(instruction[1], state);
			state->pc++;
			break;
		case 0xef: // RST 5
			CALL(0x28, state);
			break;
		case 0xf0: // RP
			if (state->flags.sign == 0) {
				RET(state);
			}
			break;
		case 0xf1: // POP PSW
			state->a = state->memory[state->sp + 1];
			temp_8 = state->memory[state->sp];
			state->flags.zero =   (0x01 == (temp_8 & 0x01));
			state->flags.sign =   (0x02 == (temp_8 & 0x02));
			state->flags.parity = (0x04 == (temp_8 & 0x04));
			state->flags.carry =  (0x08 == (temp_8 & 0x08));
			state->flags.a_carry =(0x10 == (temp_8 & 0x10));
			state->sp += 2;
			break;
		case 0xf2: // JP ADR
			if (state->flags.sign == 0) {
				state->pc = (instruction[2] << 8) | instruction[1];
			} else {
				state->pc += 2;
			}
			break;
		case 0xf3: // DI
			// TODO
			break;
		case 0xf4: // CP adr
			if (state->flags.sign == 0) {
				CALL((instruction[2] << 8) | instruction[1], state);
			} else {
				state->pc += 2;
			}
			break;
		case 0xf5: // PUSH PSW
			temp_8 = (
				state->flags.zero & 1 |
				((state->flags.sign & 1) << 1) |
				((state->flags.parity & 1) << 2) |
				((state->flags.carry & 1) << 3) | 
				((state->flags.a_carry & 1) << 4)
			);
			state->memory[state->sp - 2] = temp_8;
			state->memory[state->sp - 1] = state->a;
			state->sp -= 2;
			break;
		case 0xf6: // ORI D8
			ORA(instruction[1], state);
			state->sp++;
			break;
		case 0xf7: // RST ^
			CALL(0x30, state);
			break;
		case 0xf8: // RM
			if (state->flags.sign) {
				RET(state);
			}
			break;
		case 0xf9: // SPHL
			state->sp = join_regpair(state->h, state->l);
			break;
		case 0xfa: // JM adr
			if (state->flags.sign) {
				state->pc = (instruction[2] << 8) | instruction[1];
			} else {
				state->pc += 2;
			}
			break;
		case 0xfb: // EI
			// TODO
			break;
		case 0xfc: // CM adr
			if (state->flags.sign) {
				CALL((instruction[2] << 8) | instruction[1], state);
			} else {
				state->pc += 2;
			}
			break;
		case 0xfd: break; // NOP
		case 0xfe: // CPI D8
			CMP(instruction[1], state);
			break;
		case 0xff: // RST 7
			CALL(0x38, state);
			break;		
	}
	// display current state
	printf("Flags\n\tCarry=%d,Parity=%d,Sign=%d,Zero=%d\n",
		state->flags.carry, state->flags.parity, state->flags.sign,
		state->flags.zero);
	printf("Registers\n\tA $%02x B $%02x C $%02x D $%02x E $%02x H $%02x L $%02x SP %04x\n",
		state->a, state->b, state->c, state->d, state->e, state->h, state->l,
		state->sp);
	
	state->pc += 1;

}

void CALL(uint16_t address, emu_state_t* state)
{
	// push pc on top of stack
	// then jump to called address
	PUSH( (state->pc & 0xff), (state->pc >> 8) & 0xff, state);
	state->pc = address;
}

void PUSH(uint8_t reg_1, uint8_t reg_2, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state ptr passed to PUSH instr\n");
		exit(1);
	}
	state->memory[state->sp - 2] = reg_1;
	state->memory[state->sp - 1] = reg_2;
	state->sp -= 2;
}

void POP(uint8_t* reg_1, uint8_t* reg_2, emu_state_t* state)
{
	if (reg_1 == NULL || reg_2 == NULL) {
		fprintf(stderr, "error: Null register ptr passed to POP instr\n");
		exit(1);
	} 
	if (state == NULL) {
		fprintf(stderr, "error: Null state ptr passed to POP instr\n");
		exit(1);
	}
	*reg_1 = state->memory[state->sp];
	*reg_2 = state->memory[state->sp + 1];
	state->sp += 2;
}

void RET(emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state ptr in RET instruction\n");
		exit(1);
	}
	state->pc = (state->memory[state->sp + 1] << 8) | state->memory[state->sp];
	state->sp += 2;
}

void CMP(uint8_t reg, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer in CMP instruction\n");
		exit(1);
	}
	uint16_t answer = state->a + (~(reg) + 1);
	set_flags_arithmetic(answer, state);
}

void set_flags_logic(uint8_t value, emu_state_t* state)
{
	state->flags.zero = value == 0;
	state->flags.sign = (value & 0x80) != 0;
	state->flags.carry = state->flags.a_carry = 0;
	state->flags.parity = parity_8(value);
	state->a = value;
}

void set_flags_arithmetic(uint16_t value, emu_state_t* state)
{
	state->flags.zero = (value & 0xff) == 0;
	state->flags.sign = (value & 0x80) != 0;
	state->flags.carry = value > 0xff;
	state->flags.parity = parity_8((uint8_t)(value & 0xff));
	// state->flags.a_carry ...
}

void ORA(uint8_t reg, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer in ORA instruction\n");
		exit(1);
	}
	uint8_t answer = state->a | reg;
	set_flags_logic(answer, state);
	state->a = answer;
}

void XRA(uint8_t reg, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer in XRA instruction\n");
		exit(1);
	}
	uint8_t answer = state->a ^ reg;
	set_flags_logic(answer, state);
	state->a = answer;
}

void ANA(uint8_t reg, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer in ANA instruction\n");
		exit(1);
	}
	uint8_t answer = state->a & reg;
	set_flags_logic(answer, state);
	state->a = answer;
}

void SBB(uint8_t reg, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer in SBB instruction\n");
		exit(1);
	}
	uint16_t answer = state->a + (~(reg) + 1) + (~(state->flags.carry) + 1);
	set_flags_arithmetic(answer, state);
	state->a = answer & 0xff;
}

void SUB(uint8_t reg, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer in SUB instruction\n");
		exit(1);
	}
	uint16_t answer = state->a + (~(reg) + 1);
	set_flags_arithmetic(answer, state);
	state->a = answer & 0xff;
}

void ADC(uint8_t reg, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer in ADC instruction\n");
		exit(1);
	}
	uint16_t answer = state->a + reg + state->flags.carry;
	set_flags_arithmetic(answer, state);
	state->a = answer & 0xff;
}

void ADD(uint8_t reg, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer in ADD instruction\n");
		exit(1);
	}
	uint16_t answer = state->a + reg;
	set_flags_arithmetic(answer, state);
	state->a = answer & 0xff;
}

void MOV(uint8_t* to, uint8_t from)
{
	if (to == NULL) {
		fprintf(stderr, "error: Null 'to' register pointer in MOV instruction\n");
		exit(1);
	}
	*to = from;
}

void DCX_regpair(uint8_t* reg_1, uint8_t* reg_2)
{
	if (reg_1 == NULL) {
		fprintf(stderr, "error: Null reg_1 pointer in DCX instruction\n");
		exit(1);
	}
	if (reg_2 == NULL) {
		fprintf(stderr, "error: Null reg_2 pointer in DCX instruction\n");
		exit(1);
	}
	uint16_t temp = join_regpair(*reg_1, *reg_2) - 1;
	*reg_1 = temp >> 8;
	*reg_2 = temp & 0xff;
}

void DAD_regpair(uint8_t reg_1, uint8_t reg_2, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer in DAD instruction\n");
		exit(1);
	}
	uint32_t temp = join_regpair(state->h, state->l) + join_regpair(reg_1, reg_2);
	state->flags.carry = (temp & 0xffff0000) > 0;
	// now extract H,L
	state->h = temp >> 8;
	state->l = temp & 0xff;
}

void INX_regpair(uint8_t* reg_1, uint8_t* reg_2)
{
	if (reg_1 == NULL) {
		fprintf(stderr, "error: Null reg_1 pointer in INX instruction\n");
		exit(1);
	}
	if (reg_2 == NULL) {
		fprintf(stderr, "error: Null reg_2 pointer in INX instruction\n");
		exit(1);
	}
	uint16_t temp = join_regpair(*reg_1, *reg_2) + 1;
	*reg_1 = temp >> 8;
	*reg_2 = temp & 0xff;
}

void MVI_register(uint8_t* reg, uint8_t data, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer in MVI instruction\n");
		exit(1);
	}
	if (reg == NULL) {
		fprintf(stderr, "error: Null register pointer in MVI instruction at pc:0x%02x\n", state->pc);
		exit(1);
	}
	*reg = data;
	state->pc++;
}

void INR_register(uint8_t* reg, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer in INR instruction\n");
		exit(1);
	}
	if (reg == NULL) {
		fprintf(stderr, "error: Null register pointer in INR instruction at pc:0x%02x\n", state->pc);
		exit(1);
	}
	uint8_t temp = *reg + 1;
	state->flags.zero = (temp == 0); // check if num = 0
	state->flags.sign = (temp & 0x80); // check for MSB being set
	state->flags.carry = (*reg == 0xff); // only works for this example really
	state->flags.parity = parity_8(temp);
	*reg = temp;
}

void DCR_register(uint8_t* reg, emu_state_t* state)
{
	if (state == NULL) {
		fprintf(stderr, "error: Null state pointer in DCR instruction\n");
		exit(1);
	}
	if (reg == NULL) {
		fprintf(stderr, "error: Null register pointer in DCR instruction at pc:0x%02x\n", state->pc);
		exit(1);
	}
	uint8_t temp = *reg - 1;
	state->flags.zero = (temp == 0);
	state->flags.sign = (temp & 0x80);
	state->flags.carry = (*reg == 0);
	state->flags.parity = parity_8(temp);
	*reg = temp;
}


uint16_t join_regpair(uint8_t reg_1, uint8_t reg_2)
{
	// turns two 8 bit regs into a single 16 bit address
	return (reg_1 << 8) | reg_2;
}


/*
 note: parity is set when even number of set bits, 0 when odd
	also, this approach is not the most efficient but the
	difference is negligible.
*/
bool parity_8(uint8_t n)
{
	bool parity = true;

	while (n)
	{
		if (n & 1)
		{
			parity = !parity;
		}
		n >>= 1;
	}
	return parity;
}


