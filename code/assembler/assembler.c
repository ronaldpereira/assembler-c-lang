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
	int i, j, flag;
	char *line, *token;

	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w+r");

	binary = (bool*) calloc(8,sizeof(bool));
	line = (char*) malloc(1000*sizeof(char));
	token = (char*) malloc(50*sizeof(char));

	fprintf(output, "DEPTH = 256;\nWIDTH = 8;\nADDRESS_RADIX = BIN;\nDATA_RADIX = BIN;\nCONTENT\nBEGIN\n\n");

	i = 0;
	while(i < 256)
	{
		binaryConversion(binary, i);
		for(j = 0; j < 8; j++)
			fprintf(output, "%d", binary[j]);
		fprintf(output, "  :  \n");

		// Aqui vão ficar as instruções de leitura e tradução do input
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

			if(token[0] == '_')
			{
				//Salva o label em uma struct
				token = strtok(token, ":");
				printf("Struct = %s\n", token);
			}

			else
			{
				//Identifica uma instrução
				printf("Instrução = %s\n", token);
				//Switch case para cada instrução
				
				if(strcmp(token, "exit") == 0)
				{
					fprintf(output, "00000000\n");
					i++;
					binaryConversion(binary, i);
					for(j = 0; j < 8; j++)
						fprintf(output, "%d", binary[j]);
					fprintf(output, "  :  ");
					fprintf(output, "00000000\n");
				}
				/*
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

				else if(strcmp(token, "") == 0)
				{
					
				}
				*/
			}
		}

		i++;
	}

	fprintf(output, "END;\n");

	fprintf(stdout, "\n***PROGRAM OUTPUTS***\nFile '%s' assembled sucessfully into '%s'.\n\n", argv[1], argv[2]);

	free(binary);
	free(line);
	fclose(input);
	fclose(output);

	return 0;
}
