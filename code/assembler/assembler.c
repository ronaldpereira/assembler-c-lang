#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Address
{
	char Label[50];
	int Offset; // Valor do offset no arquivo output
} Address;

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

	if(dec == 1) // Se dec for -1, então o algoritmo abaixo não se aplica para transformação em complemento de dois
	{
		for(i = 0; i < 8; i++)
		{
			comp[i] = 1;
		}

		return comp;
	}

	dec = dec - 1; //Complemento começa em -1

	binaryConversion(comp, dec);

	// Algoritmo de transformação em complemento de dois
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
					fprintf(output, "00000000;\n");
					pc++;
					binaryConversion(binary, pc);
					for(j = 0; j < 8; j++)
						fprintf(output, "%d", binary[j]);
					fprintf(output, "  :  ");
					fprintf(output, "00000000;\n");
				}

				else if(strcmp(token, "loadi") == 0)
				{
					fprintf(output, "00001");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						dec = (int)(token[1] - '0'); // Transformação de string para inteiro
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

					//Leitura do imediato
					token = strtok(NULL, " \t");
					if(strcmp(token, "IO") == 0) // Endereço de Entrada de dados
						fprintf(output, "11111110;\n");

					else if(token[0] >= '0' && token[0] <= '9') // É um imediato positivo válido
					{
						dec = atoi(token); // Transformação de string para inteiro
						binaryConversion(binary, dec);
						for(j = 0; j < 8; j++)
						{
							fprintf(output, "%d", binary[j]);
						}
						fprintf(output, ";\n");
					}

					else if(token[0] == '-' && token[1] >= '0' && token[1] <= '9')
					{
						for(j = 0; j < (int)(strlen(token)-1); j++)
						{
							token[j] = token[j+1]; //Shifta a string em uma posição à esquerda
						}
						token[j] = '\0';
						dec = atoi(token);
						binaryTwoComplement(binary, dec);
						for(j = 0; j < 8; j++)
							fprintf(output, "%d", binary[j]);
						fprintf(output, ";\n");
					}
					/*
					else // É qualquer outro tipo de dado (pseudoinstrução .data)
					{

					}
					*/
				}

				else if(strcmp(token, "storei") == 0)
				{
					fprintf(output, "00010");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						dec = (int)(token[1] - '0'); // Transformação de string para inteiro
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

					//Leitura do imediato
					token = strtok(NULL, " \t");
					if(strcmp(token, "IO") == 0) // Endereço de Entrada de dados
						fprintf(output, "11111110;\n");

					else if(token[0] >= '0' && token[0] <= '9') // É um imediato positivo válido
					{
						dec = atoi(token); // Transformação de string para inteiro
						binaryConversion(binary, dec);
						for(j = 0; j < 8; j++)
						{
							fprintf(output, "%d", binary[j]);
						}
						fprintf(output, ";\n");
					}

					else if(token[0] == '-' && token[1] >= '0' && token[1] <= '9')
					{
						for(j = 0; j < (int)(strlen(token)-1); j++)
						{
							token[j] = token[j+1]; //Shifta a string em uma posição à esquerda
						}
						token[j] = '\0';
						dec = atoi(token);
						binaryTwoComplement(binary, dec);
						for(j = 0; j < 8; j++)
							fprintf(output, "%d", binary[j]);
						fprintf(output, ";\n");
					}
					/*
					else // É qualquer outro tipo de dado (pseudoinstrução .data)
					{

					}
					*/
				}

				else if(strcmp(token, "add") == 0)
				{
					fprintf(output, "00011");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						dec = (int)(token[1] - '0'); // Transformação de string para inteiro
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

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						dec = (int)(token[1] - '0'); // Transformação de string para inteiro
						binaryConversion(binary, dec);
						for(j = 5; j < 8; j++)
						{
							fprintf(output, "%d", binary[j]);
						}
						fprintf(output, "00000;\n");
					}

				}

				else if(strcmp(token, "subtract") == 0)
				{
					fprintf(output, "00100");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						dec = (int)(token[1] - '0'); // Transformação de string para inteiro
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

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						dec = (int)(token[1] - '0'); // Transformação de string para inteiro
						binaryConversion(binary, dec);
						for(j = 5; j < 8; j++)
						{
							fprintf(output, "%d", binary[j]);
						}
						fprintf(output, "00000;\n");
					}
				}

				else if(strcmp(token, "multiply") == 0)
				{
					fprintf(output, "00101");
				}

				else if(strcmp(token, "divide") == 0)
				{
					fprintf(output, "00110");
				}

				else if(strcmp(token, "jump") == 0)
				{
					fprintf(output, "00111");
				}

				else if(strcmp(token, "jmpz") == 0)
				{
					fprintf(output, "01000");
				}

				else if(strcmp(token, "jmpn") == 0)
				{
					fprintf(output, "01001");
				}

				else if(strcmp(token, "move") == 0)
				{
					fprintf(output, "01010");
				}

				else if(strcmp(token, "load") == 0)
				{
					fprintf(output, "01011");
				}

				else if(strcmp(token, "store") == 0)
				{
					fprintf(output, "01100");
				}

				else if(strcmp(token, "loadc") == 0)
				{
					fprintf(output, "01101");
				}

				else if(strcmp(token, "clear") == 0)
				{
					fprintf(output, "01110");
				}

				else if(strcmp(token, "moveSp") == 0)
				{
					fprintf(output, "01111");
				}

				else if(strcmp(token, "slt") == 0)
				{
					fprintf(output, "10000");
				}

				else if(strcmp(token, "call") == 0)
				{
					fprintf(output, "10001");
				}

				else if(strcmp(token, "loadSp") == 0)
				{
					fprintf(output, "10010");
				}

				else if(strcmp(token, "storeSp") == 0)
				{
					fprintf(output, "10011");
				}

				else if(strcmp(token, "ret") == 0)
				{
					fprintf(output, "10100");
				}

				else if(strcmp(token, "loadRa") == 0)
				{
					fprintf(output, "10101");
				}

				else if(strcmp(token, "storeRa") == 0)
				{
					fprintf(output, "10110");
				}

				else if(strcmp(token, "addi") == 0)
				{
					fprintf(output, "10111");
				}

				else if(strcmp(token, "sgt") == 0)
				{
					fprintf(output, "11000");
				}

				else if(strcmp(token, "seq") == 0)
				{
					fprintf(output, "11001");
				}

				else if(strcmp(token, "jmpp") == 0)
				{
					fprintf(output, "11010");
				}
			}
		}

		pc++;
	}

	fprintf(output, "END;\n");

	fclose(input);
	fclose(output);

	return 0;
}
