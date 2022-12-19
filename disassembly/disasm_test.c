#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "disasm.h"

int main(const int argc, char** argv) {
    if (argc != 2) {
        printf("usage: disasm_test filename\n");
        exit(1);
    }
    FILE* file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("error: could not open %s\n", argv[1]);
        exit(1);
    }
    fseek(file, 0L, SEEK_END);
    int fsize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    unsigned char* buffer = malloc(fsize);
    fread(buffer, fsize, 1, file);
    fclose(file);

    int pc = 0;
    while (pc < fsize) {
        pc += disasm_op_8080(buffer, pc, false);
    }
    free(buffer);

    return 0;
}