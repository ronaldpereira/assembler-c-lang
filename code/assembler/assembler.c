#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool *binaryConversion(bool *bin, int dec)
{
	int i;

	for(i = 7; i >= 0; i--)
	{
		bin[i] = dec % 2;
		dec /= 2;
	}

	return bin;
}

int main(int argc, char* argv[])
{
	FILE *input, *output;
	bool *binary;
	int i, j;

	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w+r");

	binary = (bool*) calloc(8,sizeof(bool));

	fprintf(output, "DEPTH = 256;\nWIDTH = 8;\nADDRESS_RADIX = BIN;\nDATA_RADIX = BIN;\nCONTENT\nBEGIN\n\n");

	for(i = 0; i != 256; i++)
	{
		binaryConversion(binary, i);

		for(j = 0; j < 8; j++)
			fprintf(output, "%d", binary[j]);
		fprintf(output, ":\n");

		// Aqui vão ficar as instruções de leitura e tradução do input
	}

	fprintf(output, "END;\n");

	fprintf(stdout, "\n***Program Outputs***\nFile '%s' assembled sucessfully into '%s'.\n", argv[1], argv[2]);

	free(binary);
	fclose(input);
	fclose(output);

	return 0;
}
