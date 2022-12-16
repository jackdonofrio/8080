#ifndef __DISASM_H
#define __DISASM_H

int disasm_op_8080(unsigned char* codebuffer, int pc, bool verbose);

#endif // __DISASM_H