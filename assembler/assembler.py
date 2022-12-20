"""
Converts 8080 assembly instructions into bytecode.
I wrote this to make testing the emulator easier,
so that I could just write my programs in assembly
and them assemble them into bytecode rather
than having to write in bytecode.

    author: Jack Donofrio
    date: December 20, 2022
    
"""
import sys

registers = {"b" : 0, "c" : 1, "d" : 2, "e" : 3, "h" : 4, "l" : 5, "m" : 6, "a" : 7}
# technically M is not a reg, but follows the same pattern in the instructions
pair_positions = {"b" : 0, "d" : 1, "h" : 2, "sp" : 3, "psw" : 3}

address_ops = {
    'shld' : 0x22,
    'lhld' : 0x2a,
    'sta'  : 0x32,
    'lda'  : 0x3a,
    'jnz'  : 0xc2,
    'jmp'  : 0xc3,
    'cnz'  : 0xc4,
    'jz'   : 0xca,
    'cz'   : 0xcc,
    'call' : 0xcd,
    'jnc' : 0xd2,
    'cnc' : 0xd4,
    'jc'  : 0xda,
    'cc'  : 0xdc,
    'jpo' : 0xe2,
    'cpo' : 0xe4,
    'jpe' : 0xea,
    'cpe' : 0xec,
    'jp'  : 0xf2,
    'cp'  : 0xf4,
    'jm'  : 0xfa,
    'cm'  : 0xfc
}

arith_logic_ops = {
    'add' : 0x80,
    'adc' : 0x88,
    'sub' : 0x90,
    'sbb' : 0x98,
    'ana' : 0xa0,
    'xra' : 0xa8,
    'ora' : 0xb0,
    'cmp' : 0xb8
}

misc_data_ops = {
    'adi' : 0xc6,
    'aci' : 0xce,
    'out' : 0xd3,
    'sui' : 0xd6,
    'sbi' : 0xde,
    'ani' : 0xe6,
    'xri' : 0xee,
    'ori' : 0xf6,
    'cpi' : 0xfe,
    'in'  : 0xdb
}

base_ops = {
    "inr"  : 0x04,
    "dcr"  : 0x05,
    "mvi"  : 0x06,
}


pair_base_ops = {
    "lxi"  : 0x01,
    "stax" : 0x02,
    "inx"  : 0x03,
    "dad"  : 0x09,
    "ldax" : 0x0a,
    "dcx"  : 0x0b,
    "pop"  : 0xc1,
    "push" : 0xc5
}

unique_ops = {
    "nop" : 0x00,
    "rlc" : 0x07,
    "rrc" : 0x0f,
    "ral" : 0x17,
    "rar" : 0x1f,
    "daa" : 0x27,
    "cma" : 0x2f,
    "stc" : 0x37,
    "cmc" : 0x3f,
    "hlt" : 0x76,
    "rnz" : 0xc0,
    'rz'  : 0xc8,
    'ret' : 0xc9,
    'rnc' : 0xd0,
    'rc'  : 0xd8,
    'rpo' : 0xe0,
    'xthl': 0xe3,
    'rpe' : 0xe8,
    'pchl': 0xe9,
    'xchg': 0xeb,
    'rp'  : 0xf0,
    'di'  : 0xf3,
    'rm'  : 0xf8,
    'sphl': 0xf9,
    'ei'  : 0xfb 
}

def convert_to_bytecode(instruction, line_number):
    instruction_len = len(instruction)
    if instruction_len == 0:
        return 0 # something went wrong
    op_base = instruction[0]

    if op_base in unique_ops:
        return [unique_ops[op_base]]
    
    if op_base == 'mov': # special
        r_1, r_2 = instruction[1].split(',')
        return [0x40 + 8 * registers[r_1] + registers[r_2]]

    if op_base in arith_logic_ops:
        offset = registers[instruction[1]]
        return [arith_logic_ops[op_base] + offset]

    if op_base in misc_data_ops:
        return size_2_instruction(misc_data_ops[op_base], int(instruction[1], 16))
    
    pairwise = op_base in pair_base_ops
    regwise = op_base in base_ops
    addrwise = op_base in address_ops
    if pairwise or regwise or addrwise:
        if instruction_len == 1:
            return 0 # something went wrong

        # determine what to do with data
        params = instruction[1].split(',')
        params_len = len(params)
        if params_len == 0:
            return 0 # something went wrong 

        if pairwise:
            base = pair_base_ops[op_base]
            offset = pair_positions[params[0]]
            opcode = base | (offset << 4)
        elif regwise:
            base = base_ops[op_base]
            offset = registers[params[0]]
            opcode = base + offset * 8
        else: # addrwise
            return size_3_instruction(address_ops[op_base], int(params[0], 16))
        
        if params_len == 1:
            return [opcode]
        elif params_len == 2:
            try:
                data = int(params[1], 16)
            except:
                print("error: bad hex data given to instruction", op_base, "on line", line_number)
                return 0
            if op_base == 'lxi':
                return size_3_instruction(opcode, data)
            else:
                return size_2_instruction(opcode, data)
        else:
            return 0 # something went wrong

    # ...

    # some special instructions that dont follow the other patterns per se
    if op_base == 'rst':
        return [0xc7 + 8 * int(instruction[1])]

    else:
        print("error: unknown instruction", instruction)
        return [0x0]

def size_3_instruction(opcode, data):
    return [opcode, data & 0xff, data >> 8]

def size_2_instruction(opcode, data):
    return [opcode, data]


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("usage: assembler in_filename out_filename")
        exit()
    in_filename  = sys.argv[1]
    out_filename = sys.argv[2]
    asm_file = open(in_filename, 'r')
    byte_file = open(out_filename, 'wb')
    line_number = 0
    for line in asm_file.readlines():
        line_number += 1
        line = line.strip()
        if line.startswith('#') or len(line) <= 1: # ignore any comments, blank lines
            continue
        if '#' in line:
            line = line[:line.index('#')]
        line = line.lower().split(' ')
        bytes = convert_to_bytecode(line, line_number)
        for b in bytes:
            byte_file.write(b.to_bytes(1, 'little'))
