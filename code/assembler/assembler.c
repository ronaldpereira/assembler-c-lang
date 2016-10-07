#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"
#include "binary_converter.h"
#include "function_assembler.h"

int main(int argc, char* argv[])
{
	FILE *input, *output;
	bool *binary, *datavalue;
	int pc, i, j, flag, dec;
	char *line, *token, *value;
  	lista_t data, label;

	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w+r");

	binary = (bool*) calloc(8,sizeof(bool));
	datavalue = (bool*) calloc(16,sizeof(bool));
	line = (char*) malloc(1000*sizeof(char));
	token = (char*) malloc(50*sizeof(char));
	value = (char*) malloc(50*sizeof(char));
  	aloca_lista(&data);
  	aloca_lista(&label);

	// Primeira Passada -> Ler do arquivo todo



	// Segunda Passada -> Impressão do arquivo
	fprintf(output, "DEPTH = 256;\nWIDTH = 8;\nADDRESS_RADIX = BIN;\nDATA_RADIX = BIN;\nCONTENT\nBEGIN\n\n");

	pc = 0;
	while(pc < 256)
	{
		binaryConversion(binary, pc);
		for(j = 0; j < 8; j++)
			fprintf(output, "%d", binary[j]);
		fprintf(output, "  :  ");

		flag = 0;
		while(fscanf(input, "%[^\n]\n", line) != EOF) // Lê a linha atual e detecta se a linha começa com um comentário ';'
		{
			printf("%s\n", line);
			flag = 1;
			if(line[0] != ';')
				break;
		}

		if(line[0] == '\0') // Finaliza a escrita do output
		{
			binaryConversion(binary, pc);

			fseek(output, -13, SEEK_CUR); // Volta ao inicio da linha atual do output

			fprintf(output, "[");
			for(i = 0; i < 8; i++)
				fprintf(output, "%d", binary[i]);
			fprintf(output, "..11111111]  :  00000000;\n");
			pc = 255; // PC é atualizado para 11111111
		}

		if(flag == 1)
		{
			token = strtok(line, " \t");

			if(token[0] == '_') // Identifica um Label
			{
				//Salva o label em uma lista de label
        		adiciona_elemento(&label, label.ultima);
        		grava_elemento(label.ultima, token, pc);
				printf("Label = %s\n", token);
				token = strtok(NULL, " \t");
				goto _instruction;
			}

			else if(token[strlen(token)-1] == ':') // Identifica uma pseudoinstrução .data
			{
				printf("Labeldata = %s\n", token);
				//Salva o .data em um lista value
        		adiciona_elemento(&data, data.ultima);
        		grava_elemento(data.ultima, token, pc);
				token = strtok(NULL, " \t");
				printf(".data = %s\n", token);
				if(strcmp(token, ".data") == 0)
				{
					token = strtok(NULL, " \t");
					printf("token = %s\n", token);
					value = strtok(NULL, " \t");
					printf("value = %s\n", value);

					if(value[0] >= '0' && value[0] <= '9') // É um imediato positivo válido
					{
						dec = atoi(value); // Transformação de string para inteiro
						binaryConversion16bits(datavalue, dec);
					}

					else if(value[0] == '-' && value[1] >= '0' && value[1] <= '9')
					{
						for(j = 0; j < (int)(strlen(value)-1); j++)
							value[j] = value[j+1]; //Shifta a string em uma posição à esquerda
						value[j] = '\0';
						dec = atoi(value);
						binaryTwoComplement16bits(datavalue, dec);
					}

					for(j = 0; j < 8; j++)
						fprintf(output, "%d", datavalue[j]);
					fprintf(output, ";\n");

					printaPc(output, binary, pc);

					for(i = 8; i < 16; i++)
						fprintf(output, "%d", datavalue[i]);
					fprintf(output, ";\n");
				}
			}

			else
			{
				//Identifica uma instrução
				_instruction: printf("Instrução = %s\n", token);
				//If-elseif para cada instrução

				if(strcmp(token, "exit") == 0)
				{
					fprintf(output, "00000000;\n");
					printaPc(output, binary, pc);
					fprintf(output, "00000000;\n");
				}

				else if(strcmp(token, "loadi") == 0)
				{
					fprintf(output, "00001");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do imediato
					token = strtok(NULL, " \t");
					if(strcmp(token, "IO") == 0) // Endereço de Entrada de dados
						fprintf(output, "11111110;\n");

					else if(token[0] >= '0' && token[0] <= '9') // É um imediato positivo válido
						detectaImediatoPositivo(output, token, binary);

					else if(token[0] == '-' && token[1] >= '0' && token[1] <= '9')
						detectaImediatoNegativo(output, token, binary);
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
						detectaRegistrador(output, token, binary, pc);

					//Leitura do imediato
					token = strtok(NULL, " \t");
					if(strcmp(token, "IO") == 0) // Endereço de Entrada de dados
						fprintf(output, "11111110;\n");

					else if(token[0] >= '0' && token[0] <= '9') // É um imediato positivo válido
						detectaImediatoPositivo(output, token, binary);

					else if(token[0] == '-' && token[1] >= '0' && token[1] <= '9')
						detectaImediatoNegativo(output, token, binary);
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
						detectaRegistrador(output, token, binary, pc);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						detectaRegistradorSource(output, token, binary);
						fprintf(output, "00000;\n");
					}

				}

				else if(strcmp(token, "subtract") == 0)
				{
					fprintf(output, "00100");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7
						detectaRegistrador(output, token, binary, pc);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						detectaRegistradorSource(output, token, binary);
						fprintf(output, "00000;\n");
					}
				}

				else if(strcmp(token, "multiply") == 0)
				{
					fprintf(output, "00101");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						detectaRegistradorSource(output, token, binary);
						fprintf(output, "00000;\n");
					}
				}

				else if(strcmp(token, "divide") == 0)
				{
					fprintf(output, "00110");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						detectaRegistradorSource(output, token, binary);
						fprintf(output, "00000;\n");
					}
				}

				else if(strcmp(token, "jump") == 0) // Precisa de label
				{
					fprintf(output, "00111");
				}

				else if(strcmp(token, "jmpz") == 0) // Precisa de label
				{
					fprintf(output, "01000");
				}

				else if(strcmp(token, "jmpn") == 0) // Precisa de label
				{
					fprintf(output, "01001");
				}

				else if(strcmp(token, "move") == 0)
				{
					fprintf(output, "01010");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						detectaRegistradorSource(output, token, binary);
						fprintf(output, "00000;\n");
					}
				}

				else if(strcmp(token, "load") == 0)
				{
					fprintf(output, "01011");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						detectaRegistradorSource(output, token, binary);
						fprintf(output, "00000;\n");
					}
				}

				else if(strcmp(token, "store") == 0)
				{
					fprintf(output, "01100");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						detectaRegistradorSource(output, token, binary);
						fprintf(output, "00000;\n");
					}
				}

				else if(strcmp(token, "loadc") == 0)
				{
					fprintf(output, "01101");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do imediato
					token = strtok(NULL, " \t");
					if(token[0] >= '0' && token[0] <= '9') // É um imediato positivo válido
						detectaImediatoPositivo(output, token, binary);

					else if(token[0] == '-' && token[1] >= '0' && token[1] <= '9')
						detectaImediatoNegativo(output, token, binary);
				}

				else if(strcmp(token, "clear") == 0)
				{
					fprintf(output, "01110");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						detectaRegistradorSource(output, token, binary);
						fprintf(output, ";\n");
						printaPc(output, binary, pc);
						fprintf(output, "00000000;\n");
					}
				}

				else if(strcmp(token, "moveSp") == 0)
				{
					fprintf(output, "01111000;\n");

					printaPc(output, binary, pc);

					//Leitura do imediato
					token = strtok(NULL, " \t");

					if(token[0] >= '0' && token[0] <= '9') // É um imediato positivo válido
						detectaImediatoPositivo(output, token, binary);

					else if(token[0] == '-' && token[1] >= '0' && token[1] <= '9')
						detectaImediatoNegativo(output, token, binary);
				}

				else if(strcmp(token, "slt") == 0)
				{
					fprintf(output, "10000");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistradorSource(output, token, binary);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						detectaRegistradorSource(output, token, binary);
						fprintf(output, "00;\n");
					}
				}

				else if(strcmp(token, "call") == 0) // Precisa de label
				{
					fprintf(output, "10001;\n");
				}

				else if(strcmp(token, "loadSp") == 0)
				{
					fprintf(output, "10010");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do imediato
					token = strtok(NULL, " \t");
					if(strcmp(token, "IO") == 0) // Endereço de Entrada de dados
						fprintf(output, "11111110;\n");

					else if(token[0] >= '0' && token[0] <= '9') // É um imediato positivo válido
						detectaImediatoPositivo(output, token, binary);

					else if(token[0] == '-' && token[1] >= '0' && token[1] <= '9')
						detectaImediatoNegativo(output, token, binary);
				}

				else if(strcmp(token, "storeSp") == 0)
				{
					fprintf(output, "10011");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do imediato
					token = strtok(NULL, " \t");
					if(strcmp(token, "IO") == 0) // Endereço de Entrada de dados
						fprintf(output, "11111110;\n");

					else if(token[0] >= '0' && token[0] <= '9') // É um imediato positivo válido
						detectaImediatoPositivo(output, token, binary);

					else if(token[0] == '-' && token[1] >= '0' && token[1] <= '9')
						detectaImediatoNegativo(output, token, binary);
				}

				else if(strcmp(token, "ret") == 0)
				{
					fprintf(output, "10100000;\n");
					printaPc(output, binary, pc);
					fprintf(output, "00000000;\n");
				}

				else if(strcmp(token, "loadRa") == 0)
				{
					fprintf(output, "10101000;\n");

					printaPc(output, binary, pc);

					//Leitura do imediato
					token = strtok(NULL, " \t");

					if(token[0] >= '0' && token[0] <= '9') // É um imediato positivo válido
						detectaImediatoPositivo(output, token, binary);

					else if(token[0] == '-' && token[1] >= '0' && token[1] <= '9')
						detectaImediatoNegativo(output, token, binary);
				}

				else if(strcmp(token, "storeRa") == 0)
				{
					fprintf(output, "10110000;\n");

					printaPc(output, binary, pc);

					//Leitura do imediato
					token = strtok(NULL, " \t");

					if(token[0] >= '0' && token[0] <= '9') // É um imediato positivo válido
						detectaImediatoPositivo(output, token, binary);

					else if(token[0] == '-' && token[1] >= '0' && token[1] <= '9')
						detectaImediatoNegativo(output, token, binary);
				}

				else if(strcmp(token, "addi") == 0)
				{
					fprintf(output, "10111");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do imediato
					token = strtok(NULL, " \t");
					if(token[0] >= '0' && token[0] <= '9') // É um imediato positivo válido
						detectaImediatoPositivo(output, token, binary);

					else if(token[0] == '-' && token[1] >= '0' && token[1] <= '9')
						detectaImediatoNegativo(output, token, binary);
				}

				else if(strcmp(token, "sgt") == 0)
				{
					fprintf(output, "11000");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistradorSource(output, token, binary);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						detectaRegistradorSource(output, token, binary);
						fprintf(output, "00;\n");
					}
				}

				else if(strcmp(token, "seq") == 0)
				{
					fprintf(output, "11001");

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistrador(output, token, binary, pc);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
						detectaRegistradorSource(output, token, binary);

					//Leitura do Registrador
					token = strtok(NULL, " \t");
					if(token[1] >= '0' && token[1] <= '7') // É um registrador válido (R0 até R7)
					{
						detectaRegistradorSource(output, token, binary);
						fprintf(output, "00;\n");
					}
				}

				else if(strcmp(token, "jmpp") == 0) // Precisa de label
				{
					fprintf(output, "11010");
				}
			}
		}
		line[0] = '\0'; // Reseta a posição do line[0] para ser o fim da string, para que possa ser detectado no fim do arquivo de entrada
		pc++;
	}

	fprintf(output, "END;\n");

	fclose(input);
	fclose(output);

	return 0;
}
