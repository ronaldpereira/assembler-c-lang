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

bool *binaryTwoComplement(bool *comp, int dec)
{
	int i, flag = 0;

	binaryConversion(comp, dec);

	for(i = 7; i >= 0; i--)
	{
		if(comp[i] == 1 && flag == 0)
			flag = 1;

		else if(flag == 1)
		{
			if(comp[i] == 0)
				comp[i] = 1;
			else
				comp[i] = 0;
		}
	}

	return comp;
}

int main(int argc, char* argv[])
{
	FILE *input, *output;
	bool *binary;
	int pc, j, flag, dec;
	char *line, *token;

	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w+r");

	binary = (bool*) calloc(8,sizeof(bool));
	line = (char*) malloc(1000*sizeof(char));
	token = (char*) malloc(50*sizeof(char));

	fprintf(output, "DEPTH = 256;\nWIDTH = 8;\nADDRESS_RADIX = BIN;\nDATA_RADIX = BIN;\nCONTENT\nBEGIN\n\n");

	pc = 0;
	while(pc < 256)
	{
		binaryConversion(binary, pc);
		for(j = 0; j < 8; j++)
			fprintf(output, "%d", binary[j]);
		fprintf(output, "  :  ");

		flag = 0;
		while(fscanf(input, "%[^\n]\n", line) != EOF)
		{
			printf("%s\n", line);
			flag = 1;

			if(line[0] != ';')
				break;
		}

		if(flag == 1)
		{
			token = strtok(line, " \t");

			if(token[0] == '_') // Identifica um Label
			{
				//Salva o label em uma lista de label
				token = strtok(token, ":");
				printf("Struct = %s\n", token);
			}

			/*else if(token[strlen(token)] == ':') // Identifica uma pseudoinstrução .data
			{
				//Salva o .data em um lista value
				token = strtok(NULL, " \t");
				if(strcmp(token, ".data") == 0)
				{
					token = strtok(NULL, " \t");
					for(j = 0; j < atoi(token); j++)
					{

						pc++;
					}
				}
			}*/

			else
			{
				//Identifica uma instrução
				printf("Instrução = %s\n", token);
				//If-elseif para cada instrução

				if(strcmp(token, "exit") == 0)
				{
					fprintf(output, "00000000\n");
					pc++;
					binaryConversion(binary, pc);
					for(j = 0; j < 8; j++)
						fprintf(output, "%d", binary[j]);
					fprintf(output, "  :  ");
					fprintf(output, "00000000\n");
				}

				else if(strcmp(token, "loadi") == 0)
				{
					fprintf(output, "00001");
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '8') // É um registrador válido (R0 até R8)
					{
						dec = (int)(token[1] - '0'); // Transformação de string para inteiro
						printf("%d\n", dec);
						binaryConversion(binary, dec);
						for(j = 5; j < 8; j++)
						{
							fprintf(output, "%d", binary[j]);
						}
						fprintf(output, ";\n");
						pc++;
						binaryConversion(binary, pc);
						for(j = 0; j < 8; j++)
							fprintf(output, "%d", binary[j]);
						fprintf(output, "  :  ");
					}

					token = strtok(NULL, " \t");
					if(strcmp(token, "IO") == 0)
						fprintf(output, "11111110;\n");

					else if(token[0] >= '0' && token[0] <= '9') // É um imediato positivo válido
					{
						dec = (int)(token[1] - '0'); // Transformação de string para inteiro
						binaryConversion(binary, dec);
						for(j = 5; j < 8; j++)
						{
							fprintf(output, "%d", binary[j]);
						}
						fprintf(output, ";\n");
					}

					else // É qualquer outro tipo de dado (pseudoinstrução .data)
					{

					}
				}

				else if(strcmp(token, "") == 0)
				{

				}

				else if(strcmp(token, "") == 0)
				{

				}

				else if(strcmp(token, "") == 0)
				{

				}

				else if(strcmp(token, "") == 0)
				{

				}

				else if(strcmp(token, "") == 0)
				{

				}

				else if(strcmp(token, "") == 0)
				{

				}

				else if(strcmp(token, "") == 0)
				{

				}

				else if(strcmp(token, "") == 0)
				{

				}

				else if(strcmp(token, "") == 0)
				{

				}

				else if(strcmp(token, "") == 0)
				{

				}
			}
		}

		pc++;
	}

	fprintf(output, "END;\n");

	fprintf(stdout, "\n***PROGRAM OUTPUTS***\nFile '%s' assembled sucessfully into '%s'.\n\n", argv[1], argv[2]);

	free(binary);
	free(line);
	fclose(input);
	fclose(output);

	return 0;
}
