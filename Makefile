

test:
	make -C assembler infile=testfile.asm outfile=../test_bytes
	make -C emu 
	make -C emu test testfile=../test_bytes

.PHONY: clean

clean:
	make -C emu clean


	rm test_bytes

