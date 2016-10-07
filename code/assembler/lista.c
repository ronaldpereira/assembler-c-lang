#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"

void aloca_lista(lista_t* lista)
{
	lista->cabeca = (celula_t*) calloc(1, sizeof(celula_t));
	lista->cabeca->frente = NULL;
	lista->ultima = lista->cabeca;
}

void adiciona_elemento(lista_t *lista, apontador_t pointer)
{
	apontador_t novo;

	novo = (celula_t*) calloc(1, sizeof(celula_t));
	novo->frente = pointer->frente;
	pointer->frente = novo; //ALTERA PONTEIRO PRA ULTIMA
	lista->ultima = novo; //DEIXANDO SEMPRE APONTADA A ULTIMA NULL
}

void grava_elemento(apontador_t pointer, char *text, int value)
{
	int i;

	for(i = 0; text[i] != '\0'; i++)
		pointer->registro.Label[i] = text[i];
	pointer->registro.Label[i] = '\0';

	pointer->registro.Pc = value;
}

void remove_Primeiro_elemento(lista_t* lista)
{
	apontador_t novo;

	novo = lista->cabeca->frente;
	lista->cabeca->frente = novo->frente;

	if(novo->frente == NULL)
		lista->ultima = lista->cabeca;

	free(novo);
}

apontador_t procura_elemento(apontador_t pointer, char* text)
{
	while(strcmp(text, pointer->registro.Label) != 0)
		pointer = pointer->frente;

	return pointer;
}
