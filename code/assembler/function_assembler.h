#ifndef FUNCTION_ASSEMBLER
#define FUNCTION_ASSEMBLER
#include <stdio.h>
#include <stdbool.h>

void detectaRegistrador(FILE *output, char *token, bool *binary, int pc);
void detectaRegistradorSource(FILE *output, char *token, bool *binary);
void detectaImediatoNegativo(FILE *output, char *token, bool *binary);
void detectaImediatoPositivo(FILE *output, char *token, bool *binary);
void printaPc(FILE *output, bool *binary, int pc);

#endif
