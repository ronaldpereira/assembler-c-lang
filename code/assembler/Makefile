CC = gcc
CFLAGS = -Wall -Wextra -g -O0
INPUTPATH = ../tst/teste.a
OUTPUTPATH = ../tst/output.mif

all: assembler.exe

assembler.exe:
	$(CC) $(CFLAGS) assembler.c -o assembler.exe
	@echo "\n***IMPORTANT***\nBe sure to include the input and output file path in the argument of the Assembler program\nExample: ./assembler.exe ../tst/input.a ../tst/output.mif\n"

clean:
	rm assembler.exe
	@echo "\nThe assembler executable has been sucessfully removed\n"

test:
	./assembler.exe $(INPUTPATH) $(OUTPUTPATH)

compile:
	make clean && make all
