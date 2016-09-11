CC = gcc
CFLAGS = -Wall -Wextra -g -O3
TESTPATH = ../tst/teste.a

all: assembler.exe

assembler.exe:
	$(CC) $(CFLAGS) assembler.c -lm -o assembler.exe
	@echo "\n***IMPORTANT***\nBe sure to include the input and output file path in the argument of the Assembler program\nExample: ./assembler.exe ../tst/input.a ../tst/output.mif"

clean:
	rm assembler.exe
	@echo "\nThe assembler executable has been sucessfully removed"

test:
	make all && ./assembler.exe $(TESTPATH) && make clean
