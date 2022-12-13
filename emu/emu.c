/*

8080 Emulator

TODO:
	finish func for decoding/execution
		create general functions for similar ops
	flesh out algorithm for emulation
		fetch
		decode   ] these two are done in func described above 
		execute  ]
*/


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct flag_set {
	uint8_t zero;
	uint8_t sign;
	uint8_t carry;
	uint8_t parity;
	/* ... */
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



void unrecognized_instruction(emu_state_t* state);
void emulate_op(emu_state_t* state);
uint16_t join_regpair(uint8_t reg_1, uint8_t reg_2);
bool parity_8(uint8_t n);

int main(const int argc, char** argv)
{
	return 0;
}












void unrecognized_instruction(emu_state_t* state)
{
	if (state == NULL) {
		return;
	}
	printf("instruction 0x%02x not recognized\n", state->memory[state->pc]);
	state->pc -= 1;
	exit(1);
}

void emulate_op(emu_state_t* state)
{
	unsigned char* instruction = &state->memory[state->pc];
	unsigned char opcode = *instruction;
	
	uint16_t bc;

	uint32_t temp_32;
	uint16_t temp_16;
	uint8_t  temp_8;
	bool     temp_1;

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
			bc = join_regpair(state->b, state->c) + 1;
			state->b = bc >> 8;
			state->c = bc & 0xff;
			// need to determine if this should update flags
			
			break;
		case 0x04: // INR B // B <- B + 1
			temp_8 = state->b + 1;
			state->flags.zero = (temp_8 == 0); // check if num = 0
			state->flags.sign = (temp_8 & 0x80); // check for MSB being set
			state->flags.carry = (state->b == 0xff); // only works for this example really
			state->flags.parity = parity_8(temp_8);
			state->b = temp_8;
			break;
		case 0x05: // DCR B // B <- B - 1 
			temp_8 = state->b - 1;
			state->flags.zero = (temp_8 == 0);
			state->flags.sign = (temp_8 & 0x80);
			state->flags.carry = (state->b == 0); // specific to this instr
			state->flags.parity = parity_8(temp_8);
			state->b = temp_8;
			break;
		case 0x06: // MVI B,D8
			state->b = instruction[1];
			state->pc++;
			break;
		case 0x07: // RLC
			temp_1 = state->a >> 7;
			state->a = (state->a << 1) | temp_1;
			state->flags.carry = temp_1;
			break;
		case 0x08: break; // NOP
		case 0x09: // DAD B // HL <- HL + BC
			temp_32 = join_regpair(state->h, state->l) + join_regpair(state->b, state->c);
			state->flags.carry = (temp_32 & 0xffff0000) > 0;
			// now extract H,L
			state->h = temp_32 >> 8;
			state->l = temp_32 & 0xff;
			break;
		case 0x0a: // LDAX B // A <- (BC)
			state->a = state->memory[join_regpair(state->b, state->c)];
			break;
		case 0x0b: // DCX B // BC <- BC - 1
			temp_16 = join_regpair(state->b, state->c) - 1;
			state->b = temp_16 >> 8;
			state->c = temp_16 & 0xff;
			break;
		case 0x0c: // INR C 
			
			// todo - write general function for INR ops
			break;
		
	}
	state->pc += 1;




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


