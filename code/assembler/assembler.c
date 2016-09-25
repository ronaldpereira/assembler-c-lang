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
	int dec;
	int i;

	binary = (bool*) calloc(8,sizeof(bool));

	printf("Insira um numero inteiro:\n>");
	scanf("%d", &dec);

	binaryConversion(binary, dec);

	for(i = 0; i < 8; i++)
		printf("%d", binary[i]);


	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w+r");



	// Input fscanf

	//Output fprint
	fprintf(output, "DEPTH = 256;\nWIDTH = 8;\nADDRESS_RADIX = BIN;\nDATA_RADIX = BIN;\nCONTENT\nBEGIN\n\n");

	//Assembly impression

	fprintf(output, "END;\n");

	fprintf(stdout, "\n***Program Outputs***\nFile '%s' assembled sucessfully into '%s'.\n", argv[1], argv[2]);

	free(binary);
	fclose(input);
	fclose(output);

	return 0;
}
