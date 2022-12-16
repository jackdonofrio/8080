/* 
8080 bytecode -> assembly converter

algorithm adapted from Emulator 101:
 1 read bytecode into buffer
 2 get ptr to start of buffer
 3 use byte at ptr to determine opcode
 4 print name of opcode using bytes after opcode as data if applicable
 5 increment ptr by # of bytes used by instruction
 6 if at end of buffer, exit, otherwise repeat step 3 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int disasm_op_8080(unsigned char* codebuffer, int pc, bool verbose) {
    unsigned char* instruction = &codebuffer[pc];
    int size = 1; // number of bytes in opcode
    printf("%04x ", pc);
    switch (*instruction) // turn bytecode instruction into 8080
    {
    case 0x00:
        printf("NOP");
        if (verbose) {
            printf(" # does nothing");
        }
        break;
    case 0x01:
        printf("LXI    B,#$%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # loads 16-bit immediate val into reg pair BC (byte 3 ->B, byte 2 ->C)");
        }
        size = 3;
        break;
    case 0x02:
        printf("STAX   B");
        if (verbose) {
            printf(" # stores reg A (acc.) content into mem addr (BC)");
        }
        break;
    case 0x03:
        printf("INX    B");
        if (verbose) {
            printf(" # increments BC reg pair by 1");
        }
        break;
    case 0x04:
        printf("INR    B");
        if (verbose) {
            printf(" # increments reg B by 1");
        }
        break;
    case 0x05:
        printf("DCR    B");
        if (verbose) {
            printf(" # decrements reg B by 1");
        }
        break;
    case 0x06:
        printf("MVI    B,#$%02x", instruction[1]);
        if (verbose) {
            printf(" # moves byte into B");
        }
        size = 2;
        break;
    case 0x07:
        printf("RLC");
        if (verbose) {
            printf(" # rotates reg A left by 1 bit circularly");
        }
        break;
    case 0x08:
        printf("NOP");
        break;
    case 0x09:
        printf("DAD    B");
        if (verbose) {
            printf(" # increments HL register by value of BC reg pair");
        }
        break;
    case 0x0a:
        printf("LDAX   B");
        if (verbose) {
            printf(" # loads from memory at addr BC into reg A");
        }
        break;
    case 0x0b:
        printf("DCX    B");
        if (verbose) {
            printf(" # decrements BC by 1");
        }
        break;
    case 0x0c:
        printf("INR    C");
        if (verbose) {
            printf(" # increments reg C by 1");
        }
        break;
    case 0x0d:
        printf("DCR    C");
        if (verbose) {
            printf(" # decrements reg C by 1");
        }
        break;
    case 0x0e:
        printf("MVI    C,#$%02x", instruction[1]);
        if (verbose) {
            printf(" # moves byte into C");
        }
        size = 2;
        break;
    case 0x0f:
        printf("RRC");
        if (verbose) {
            printf(" # rotates A right by 1 bit (circular)");
        }
        break;
    case 0x10:
        printf("NOP");
        break;
    case 0x11:
        printf("LXI    D,#$%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # loads byte 3 into D, byte 2 into E");
        }
        size = 3;
        break;
    case 0x12:
        printf("STAX   D");
        if (verbose) {
            printf(" # loads reg A value into memory at (DE)");
        }
        break;
    case 0x13:
        printf("INX    D");
        if (verbose) {
            printf(" # increments DE pair by 1");
        }
        break;
    case 0x14:
        printf("INR    D");
        if (verbose) {
            printf(" # increments D by 1");
        }
        break;
    case 0x15:
        printf("DCR    D");
        if (verbose) {
            printf(" # decrements D by 1");
        }
        break;
    case 0x16:
        printf("MVI    D,#$%02x", instruction[1]);
        if (verbose) {
            printf(" # moves byte 2 into D");
        }
        size = 2;
        break;
    case 0x17:
        printf("RAL");
        if (verbose) {
            printf(" # rotates reg A left 1 bit");
        }
        break;
    case 0x18:
        printf("NOP");
        break;
    case 0x19:
        printf("DAD    D");
        if (verbose) {
            printf(" # adds DE pair to HL reg");
        }
        break;
    case 0x1a:
        printf("LDAX   D");
        if (verbose) {
            printf(" # loads memory at (DE) into A reg");
        }
        break;
    case 0x1b:
        printf("DCX    D");
        if (verbose) {
            printf(" # decrements DE by 1");
        }
        break;
    case 0x1c:
        printf("INR    E");
        if (verbose) {
            printf(" # increments E by 1");
        }
        break;
    case 0x1d:
        printf("DCR    E");
        if (verbose) {
            printf(" # decrements E by 1");
        }
        break;
    case 0x1e:
        printf("MVI    E,#$%02x", instruction[1]);
        if (verbose) {
            printf(" # moves byte 2 into reg E");
        }
        size = 2;
        break;
    case 0x1f:
        printf("RAR");
        if (verbose) {
            printf(" # rotates A right 1 bit");
        }
        break;
    case 0x20:
        printf("RIM");
        if (verbose) {
            printf(" # Read Interrupt Mask and serial data into A");
        }
        break;
    case 0x21:
        printf("LXI    H,#$%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # loads byte 3 into H, byte 2 into L");
        }
        size = 3;
        break;
    case 0x22:
        printf("SHLD   $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # loads L into memory at addr, H into mem at addr+1");
        }
        size = 3;
        break;
    case 0x23:
        printf("INX    H");
        if (verbose) {
            printf(" # increments HL by 1");
        }
        break;
    case 0x24:
        printf("INR    H");
        if (verbose) {
            printf(" # increments H by 1");
        }
        break;
    case 0x25:
        printf("DCR    H");
        if (verbose) {
            printf(" # decrements H by 1");
        }
        break;
    case 0x26:
        printf("MVI    H,#$%02x", instruction[1]);
        if (verbose) {
            printf(" # moves byte 2 into H");
        }
        size = 2;
        break;
    case 0x27:
        printf("DAA");
        if (verbose) {
            printf(" # Decimal Adjust Accumulator");
        }
        break;
    case 0x28:
        printf("NOP");
        break;
    case 0x29:
        printf("DAD    H");
        if (verbose) {
            printf(" # doubles HL");
        }
        break;
    case 0x2a:
        printf("LHLD   $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # loads mem data at (adr) into L, data at (adr+1) into H ");
        }
        size = 3;
        break;
    case 0x2b:
        printf("DCX    H");
        if (verbose) {
            printf(" # decrements H by 1");
        }
        break;
    case 0x2c:
        printf("INR    L");
        if (verbose) {
            printf(" # increments L by 1");
        }
        break;
    case 0x2d:
        printf("DCR    L");
        if (verbose) {
            printf(" # decrements L by 1");
        }
        break;
    case 0x2e:
        printf("MVI    L,#$%02x", instruction[1]);
        if (verbose) {
            printf(" # loads byte 2 into L");
        }
        size = 2;
        break;
    case 0x2f:
        printf("CMA");
        if (verbose) {
            printf(" # sets A to A complement");
        }
        break;
    case 0x30:
        printf("NOP");
        break;
    case 0x31:
        printf("LXI    SP,#$%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # loads byte 3 into stack ptr hi byte, byte 2 -> SP lo byte");
        }
        size = 3;
        break;
    case 0x32:
        printf("STA    $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # loads value of A into memory at (adr)");
        }
        size = 3;
        break;
    case 0x33:
        printf("INX    SP");
        if (verbose) {
            printf(" # increments stack pointer by 1");
        }
        break;
    case 0x34:
        printf("INR    M");
        if (verbose) {
            printf(" # increments memory stored at (HL) by 1");
        }
        break;
    case 0x35:
        printf("DCR    M");
        if (verbose) {
            printf(" # decrements memory stored at (HL) by 1");
        }
        break;
    case 0x36:
        printf("MVI    M,#$%02x", instruction[1]);
        if (verbose) {
            printf(" # load byte 2 into HL");
        }
        size = 2;
        break;
    case 0x37:
        printf("STC");
        if (verbose) {
            printf(" # sets carry flag to 1");
        }
        break;
    case 0x38:
        printf("NOP");
        break;
    case 0x39:
        printf("DAD    SP");
        if (verbose) {
            printf(" # increments HL by SP");
        }
        break;
    case 0x3a:
        printf("LDA    $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # load memory at addr into A");
        }
        size = 3;
        break;
    case 0x3b:
        printf("DCX    SP");
        if (verbose) {
            printf(" # decrement stack ptr by 1");
        }
        break;
    case 0x3c:
        printf("INR    A");
        if (verbose) {
            printf(" # increments A by 1");
        }
        break;
    case 0x3d:
        printf("DCR    A");
        if (verbose) {
            printf(" # decrements A by 1");
        }
        break;
    case 0x3e:
        printf("MVI    A,#$%02x", instruction[1]);
        if (verbose) {
            printf(" # moves byte 2 into A");
        }
        size = 2;
        break;
    case 0x3f:
        printf("CMC");
        if (verbose) {
            printf(" # sets carry flag to complement of itself");
        }
        break;
    case 0x40:
        printf("MOV    B,B");
        if (verbose) {
            printf(" # stores B in B");
        }
        break;
    case 0x41:
        printf("MOV    B,C");
        if (verbose) {
            printf(" # stores C in B");
        }
        break;
    case 0x42:
        printf("MOV    B,D");
        if (verbose) {
            printf(" # stores D in B");
        }
        break;
    case 0x43:
        printf("MOV    B,E");
        if (verbose) {
            printf(" # stores E in B");
        }
        break;
    case 0x44:
        printf("MOV    B,H");
        if (verbose) {
            printf(" # stores H in B");
        }
        break;
    case 0x45:
        printf("MOV    B,L");
        if (verbose) {
            printf(" # stores L in B");
        }
        break;
    case 0x46:
        printf("MOV    B,M");
        if (verbose) {
            printf(" # stores memory value from (HL) into B");
        }
        break;
    case 0x47:
        printf("MOV    B,A");
        if (verbose) {
            printf(" # stores A in B");
        }
        break;
    case 0x48:
        printf("MOV    C,B");
        if (verbose) {
            printf(" # stores B in C");
        }
        break;
    case 0x49:
        printf("MOV    C,C");
        if (verbose) {
            printf(" # stores C in C");
        }
        break;
    case 0x4a:
        printf("MOV    C,D");
        break;
    case 0x4b:
        printf("MOV    C,E");
        break;
    case 0x4c:
        printf("MOV    C,H");
        break;
    case 0x4d:
        printf("MOV    C,L");
        break;
    case 0x4e:
        printf("MOV    C,M");
        break;
    case 0x4f:
        printf("MOV    C,A");
        break;
    case 0x50:
        printf("MOV    D,B");
        break;
    case 0x51:
        printf("MOV    D,C");
        break;
    case 0x52:
        printf("MOV    D,D");
        break;
    case 0x53:
        printf("MOV    D,E");
        break;
    case 0x54:
        printf("MOV    D,H");
        break;
    case 0x55:
        printf("MOV    D,L");
        break;
    case 0x56:
        printf("MOV    D,M");
        break;
    case 0x57:
        printf("MOV    D,A");
        break;
    case 0x58:
        printf("MOV    E,B");
        break;
    case 0x59:
        printf("MOV    E,C");
        break;
    case 0x5a:
        printf("MOV    E,D");
        break;
    case 0x5b:
        printf("MOV    E,E");
        break;
    case 0x5c:
        printf("MOV    E,H");
        break;
    case 0x5d:
        printf("MOV    E,L");
        break;
    case 0x5e:
        printf("MOV    E,M");
        break;
    case 0x5f:
        printf("MOV    E,A");
        break;
    case 0x60:
        printf("MOV    H,B");
        break;
    case 0x61:
        printf("MOV    H,C");
        break;
    case 0x62:
        printf("MOV    H,D");
        break;
    case 0x63:
        printf("MOV    H,E");
        break;
    case 0x64:
        printf("MOV    H,H");
        break;
    case 0x65:
        printf("MOV    H,L");
        break;
    case 0x66:
        printf("MOV    H,M");
        break;
    case 0x67:
        printf("MOV    H,A");
        break;
    case 0x68:
        printf("MOV    L,B");
        break;
    case 0x69:
        printf("MOV    L,C");
        break;
    case 0x6a:
        printf("MOV    L,D");
        break;
    case 0x6b:
        printf("MOV    L,E");
        break;
    case 0x6c:
        printf("MOV    L,H");
        break;
    case 0x6d:
        printf("MOV    L,L");
        break;
    case 0x6e:
        printf("MOV    L,M");
        break;
    case 0x6f:
        printf("MOV    L,A");
        break;
    case 0x70:
        printf("MOV    M,B");
        break;
    case 0x71:
        printf("MOV    M,C");
        break;
    case 0x72:
        printf("MOV    M,D");
        break;
    case 0x73:
        printf("MOV    M,E");
        break;
    case 0x74:
        printf("MOV    M,H");
        break;
    case 0x75:
        printf("MOV    M,L");
        break;
    case 0x76:
        printf("HLT");
        if (verbose) {
            printf(" # halts CPU");
        }
        break;
    case 0x77:
        printf("MOV    M,A");
        break;
    case 0x78:
        printf("MOV    A,B");
        break;
    case 0x79:
        printf("MOV    A,C");
        break;
    case 0x7a:
        printf("MOV    A,D");
        break;
    case 0x7b:
        printf("MOV    A,E");
        break;
    case 0x7c:
        printf("MOV    A,H");
        break;
    case 0x7d:
        printf("MOV    A,L");
        break;
    case 0x7e:
        printf("MOV    A,M");
        break;
    case 0x7f:
        printf("MOV    A,A");
        break;
    case 0x80:
        printf("ADD    B");
        break;
    case 0x81:
        printf("ADD    C");
        break;
    case 0x82:
        printf("ADD    D");
        break;
    case 0x83:
        printf("ADD    E");
        break;
    case 0x84:
        printf("ADD    H");
        break;
    case 0x85:
        printf("ADD    L");
        break;
    case 0x86:
        printf("ADD    M");
        break;
    case 0x87:
        printf("ADD    A");
        break;
    case 0x88:
        printf("ADC    B");
        break;
    case 0x89:
        printf("ADC    C");
        break;
    case 0x8a:
        printf("ADC    D");
        break;
    case 0x8b:
        printf("ADC    E");
        break;
    case 0x8c:
        printf("ADC    H");
        break;
    case 0x8d:
        printf("ADC    L");
        break;
    case 0x8e:
        printf("ADC    M");
        break;
    case 0x8f:
        printf("ADC    A");
        break;
    case 0x90:
        printf("SUB    B");
        break;
    case 0x91:
        printf("SUB    C");
        break;
    case 0x92:
        printf("SUB    D");
        break;
    case 0x93:
        printf("SUB    E");
        break;
    case 0x94:
        printf("SUB    H");
        break;
    case 0x95:
        printf("SUB    L");
        break;
    case 0x96:
        printf("SUB    M");
        break;
    case 0x97:
        printf("SUB    A");
        break;
    case 0x98:
        printf("SBB    B");
        break;
    case 0x99:
        printf("SBB    C");
        break;
    case 0x9a:
        printf("SBB    D");
        break;
    case 0x9b:
        printf("SBB    E");
        break;
    case 0x9c:
        printf("SBB    H");
        break;
    case 0x9d:
        printf("SBB    L");
        break;
    case 0x9e:
        printf("SBB    M");
        break;
    case 0x9f:
        printf("SBB    A");
        break;
    case 0xa0:
        printf("ANA    B");
        break;
    case 0xa1:
        printf("ANA    C");
        break;
    case 0xa2:
        printf("ANA    D");
        break;
    case 0xa3:
        printf("ANA    E");
        break;
    case 0xa4:
        printf("ANA    H");
        break;
    case 0xa5:
        printf("ANA    L");
        break;
    case 0xa6:
        printf("ANA    M");
        break;
    case 0xa7:
        printf("ANA    A");
        break;
    case 0xa8:
        printf("XRA    B");
        break;
    case 0xa9:
        printf("XRA    C");
        break;
    case 0xaa:
        printf("XRA    D");
        break;
    case 0xab:
        printf("XRA    E");
        break;
    case 0xac:
        printf("XRA    H");
        break;
    case 0xad:
        printf("XRA    L");
        break;
    case 0xae:
        printf("XRA    M");
        break;
    case 0xaf:
        printf("XRA    A");
        break;
    case 0xb0:
        printf("ORA    B");
        break;
    case 0xb1:
        printf("ORA    C");
        break;
    case 0xb2:
        printf("ORA    D");
        break;
    case 0xb3:
        printf("ORA    E");
        break;
    case 0xb4:
        printf("ORA    H");
        break;
    case 0xb5:
        printf("ORA    L");
        break;
    case 0xb6:
        printf("ORA    M");
        break;
    case 0xb7:
        printf("ORA    A");
        break;
    case 0xb8:
        printf("CMP    B");
        break;
    case 0xb9:
        printf("CMP    C");
        break;
    case 0xba:
        printf("CMP    D");
        break;
    case 0xbb:
        printf("CMP    E");
        break;
    case 0xbc:
        printf("CMP    H");
        break;
    case 0xbd:
        printf("CMP    L");
        break;
    case 0xbe:
        printf("CMP    M");
        break;
    case 0xbf:
        printf("CMP    A");
        break;
    case 0xc0:
        printf("RNZ");
        if (verbose) {
            printf(" # return if zero flag not set");
        }
        break;
    case 0xc1:
        printf("POP    B");
        if (verbose) {
            printf(" # pop from stack into reg pair BC");
        }
        break;
    case 0xc2:
        printf("JNZ    $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # jump to address if zero flag not set");
        }
        size = 3;
        break;
    case 0xc3:
        printf("JMP    $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # jump to adr");
        }
        size = 3;
        break;
    case 0xc4:
        printf("CNZ    $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # call adr if zero flag not set");
        }
        size = 3;
        break;
    case 0xc5:
        printf("PUSH   B");
        if (verbose) {
            printf(" # push contents of BC reg to stack");
        }
        break;
    case 0xc6:
        printf("ADI    #$%02x", instruction[1]);
        if (verbose) {
            printf(" # add immediate value to reg A");
        }
        size = 2;
        break;
    case 0xc7:
        printf("RST    0");
        if (verbose) {
            printf(" # call $0 (restart subroutine)");
        }
        break;
    case 0xc8:
        printf("RZ");
        if (verbose) {
            printf(" # return if zero flag set");
        }
        break;
    case 0xc9:
        printf("RET");
        if (verbose) {
            printf(" # return from subroutine");
        }
        break;
    case 0xca:
        printf("JZ     $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # if zero flag set, jump to adr");
        }
        size = 3;
        break;
    case 0xcb:
        printf("NOP");
        break;
    case 0xcc:
        printf("CZ     $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # if zero flag set, call adr");
        }
        size = 3;
        break;
    case 0xcd:
        printf("CALL   $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # call adr");
        }
        size = 3;
        break;
    case 0xce:
        printf("ACI    #$%02x", instruction[1]);
        if (verbose) {
            printf(" # add immediate value to A with carry");
        }
        size = 2;
        break;
    case 0xcf:
        printf("RST    1");
        if (verbose) {
            printf(" # equivalent to CALL $8");
        }
        break;
    case 0xd0:
        printf("RNC");
        if (verbose) {
            printf(" # return if carry flag not set");
        }
        break;
    case 0xd1:
        printf("POP    D");
        if (verbose) {
            printf(" # pop from stack into reg pair DE");
        }
        break;
    case 0xd2:
        printf("JNC    $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # jump to addr if carry flag not set");
        }
        size = 3;
        break;
    case 0xd3:
        printf("OUT    #$%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # output A to port");
        }
        size = 3;
        break;
    case 0xd4:
        printf("CNC    $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # call adr if carry flag not set");
        }
        size = 3;
        break;
    case 0xd5:
        printf("PUSH   D");
        if (verbose) {
            printf(" # push from top of stack to reg pair DE");
        }
        break;
    case 0xd6:
        printf("SUI    #$%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # subtract immediate value from A");
        }
        size = 3;
        break;
    case 0xd7:
        printf("RST    2");
        if (verbose) {
            printf(" # call $0x10");
        }
        break;
    case 0xd8:
        printf("RC");
        if (verbose) {
            printf(" # return if carry flag is set");
        }
        break;
    case 0xd9:
        printf("NOP");
        break;
    case 0xda:
        printf("JC     $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # if carry flag set, jump to addr");
        }
        size = 3;
        break;
    case 0xdb:
        printf("IN     #$%02x", instruction[1]);
        if (verbose) {
            printf(" # input port to A");
        }
        size = 2;
        break;
    case 0xdc:
        printf("CC     $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # if carry flag set, call addr");
        }
        size = 3;
        break;
    case 0xdd:
        printf("NOP");
        break;
    case 0xde:
        printf("SBI    #$%02x", instruction[1]);
        if (verbose) {
            printf(" # Subtract immediate value from A with Borrow");
        }
        size = 2;
        break;
    case 0xdf:
        printf("RST    3");
        if (verbose) {
            printf(" # call $0x18");
        }
        break;
    case 0xe0:
        printf("RPO");
        if (verbose) {
            printf(" # if parity odd, return");
        }
        break;
    case 0xe1:
        printf("POP    H");
        if (verbose) {
            printf(" # pop into HL reg");
        }
        break;
    case 0xe2:
        printf("JPO    $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # jump to addr if parity odd");
        }
        size = 3;
        break;
    case 0xe3:
        printf("XTHL");
        if (verbose) {
            printf(" # Exchange Top of stack (two bytes addressed by SP+1 and SP) with HL");
        }
        break;
    case 0xe4:
        printf("CPO    $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # call adr if parity flag");
        }
        size = 3;
        break;
    case 0xe5:
        printf("PUSH   H");
        if (verbose) {
            printf(" # push reg pair HL to top of stack");
        }
        break;
    case 0xe6:
        printf("ANI    #$%02x", instruction[1]);
        if (verbose) {
            printf(" # AND A with immediate value");
        }
        size = 2;
        break;
    case 0xe7:
        printf("RST    4");
        if (verbose) {
            printf(" # call $0x20");
        }
        break;
    case 0xe8:
        printf("RPE");
        if (verbose) {
            printf(" # return if parity even");
        }
        break;
    case 0xe9:
        printf("PCHL");
        if (verbose) {
            printf(" # Load into program counter from HL");
        }
        break;
    case 0xea:
        printf("JPE    $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # jump if party even to addr");
        }
        size = 3;
        break;
    case 0xeb:
        printf("XCHG");
        if (verbose) {
            printf(" # exchange DE and HL");
        }
        break;
    case 0xec:
        printf("CPE    $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # Call if Parity Even");
        }
        size = 3;
        break;
    case 0xed:
        printf("NOP");
        break;
    case 0xee:
        printf("XRI    #$%02x", instruction[1]);
        if (verbose) {
            printf(" # XOR A with immediate value");
        }
        size = 2;
        break;
    case 0xef:
        printf("RST    5");
        if (verbose) {
            printf(" # call $0x28");
        }
        break;
    case 0xf0:
        printf("RP");
        if (verbose) {
            printf(" # return if plus (S not set)");
        }
        break;
    case 0xf1:
        printf("POP    PSW");
        if (verbose) {
            printf(" # pop into register pair constructed from Accumulator A and flags");
        }
        break;
    case 0xf2:
        printf("JP     $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # jump to addr if plus (S not set)");
        }
        size = 3;
        break;
    case 0xf3:
        printf("DI");
        if (verbose) {
            printf(" # disable interrupts");
        }
        break;
    case 0xf4:
        printf("CP     $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # call if plus (S not set)");
        }
        size = 3;
        break;
    case 0xf5:
        printf("PUSH   PSW");
        if (verbose) {
            printf(" # push onto stack from register pair constructed from acc. A and flags");
        }
        break;
    case 0xf6:
        printf("ORI    #$%02x", instruction[1]);
        if (verbose) {
            printf(" # OR A with immediate value");
        }
        size = 2;
        break;
    case 0xf7:
        printf("RST    6");
        if (verbose) {
            printf(" # call $0x30");
        }
        break;
    case 0xf8:
        printf("RM");
        if (verbose) {
            printf(" # return if minus (S set)");
        }
        break;
    case 0xf9:
        printf("SPHL");
        if (verbose) {
            printf(" # Load SP from HL");
        }
        break;
    case 0xfa:
        printf("JM     $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # jump to adr if minus");
        }
        size = 3;
        break;
    case 0xfb:
        printf("EI");
        if (verbose) {
            printf(" # enable interrupts");
        }
        break;
    case 0xfc:
        printf("CM     $%02x%02x", instruction[2], instruction[1]);
        if (verbose) {
            printf(" # call adr if minus");
        }
        size = 3;
        break;
    case 0xfd:
        printf("NOP");
        break;
    case 0xfe:
        printf("CPI    #$%02x", instruction[1]);
        if (verbose) {
            printf(" # Compare A with immediate value");
        }
        size = 2;
        break;
    case 0xff:
        printf("RST    7");
        if (verbose) {
            printf(" # call $0x38");
        }
        break;
    }
    printf("\n");
    return size;
}
