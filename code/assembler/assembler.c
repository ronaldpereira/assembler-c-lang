#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char* argv[])
{
	FILE *input, *output;

	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w+r");

	// Input fscanf

	//Output fprint
	fprintf(output, "DEPTH = 256;\nWIDTH = 8;\nADDRESS_RADIX = BIN;\nDATA_RADIX = BIN;\nCONTENT\nBEGIN\n\n");

	//Assembly impression

	fprintf(output, "END;\n");

	fprintf(stdout, "\n***Program Outputs***\nFile '%s' assembled sucessfully into '%s'.\n", argv[1], argv[2]);

	return 0;
}
