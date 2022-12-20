L = ../disassembly

emu:
	gcc -I$L $L/disasm.c emu.c -o emu

.PHONY: clean test

test:
	./emu $(testfile) debug

clean:
	rm emu
