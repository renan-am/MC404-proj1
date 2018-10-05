#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

//a-z (0-25); . (26); _ (27); 0-9 (28)
#define ALFABETO_TAM 29 //(a-z + _ + . + (0-9) ) 

typedef struct TRIENODE{
	struct TRIENODE *prox[ALFABETO_TAM];
	int forma_palavra;
	Token *token;
} TRIENODE;

int getIndice(char alvo){
	if (alvo >= 97 && alvo <= 122)
		return (alvo - 'a');
	else if (alvo >= 65 && alvo <= 90)
		return (alvo - 'A');
	else if (alvo == '.')
		return 26;
	else if (alvo == '_')
		return 27;
	else if (alvo >= 48 && alvo <= 57)
		return 28;
	else
		return -1;
}
/*
char getLetra(int i){
	if (i >= 0 || i <= 25)
		return (i + 'a');
	else if (i == 26)
		return '.';
	else if (i == 27)
		return '_';
	else
		return 0;
}
*/

static Token *novoToken (char *palavra, TipoDoToken tipo, unsigned linha){
    int tam = strlen(palavra);
    
    Token *temp = malloc(sizeof(Token));
    
    temp->palavra = malloc((tam+1)*sizeof(char));
    strcpy(temp->palavra, palavra);

    temp->tipo = tipo;
    temp->linha = linha;

    return temp;
}

void inserirTrie (TRIENODE *raiz, TipoDoToken tipo, char *palavra){
	int tam = strlen(palavra);
	int indice;

	TRIENODE *aux = raiz;

	for (int i = 0; i < tam; i++){
		indice = getIndice(palavra[i]);
		
		if (!aux->prox[indice]){
			TRIENODE *temp = malloc (sizeof(TRIENODE));
			temp->forma_palavra = 0;
			temp->token = NULL;
			
			for (int j = 0; j < ALFABETO_TAM; j++)
				temp->prox[j] = NULL;

			aux->prox[indice] = temp;
		}

		aux = aux->prox[indice];
	}

	aux->forma_palavra = 1;
	aux->token = malloc(sizeof(Token));
	aux->token->palavra = malloc((tam+1)*sizeof(char));
	strcpy (aux->token->palavra, palavra);
	aux->token->tipo = tipo;
}

Token *buscarTrie(TRIENODE *raiz, char *palavra, unsigned linha){
	int tam = strlen(palavra);
	int indice;

	TRIENODE *aux = raiz;

	for (int i = 0; i < tam; i++){
		indice = getIndice(palavra[i]);
		if (indice < 0)
			break;

		if (!aux->prox[indice]){
			if (palavra[0] >= 48 && palavra[0] <= 57){
				if (tam >= 2 && palavra[1] == 'x')
					return novoToken(palavra, Hexadecimal, linha);
				else
					return novoToken(palavra, Decimal, linha);
			}
			
			return NULL;
		}

		aux = aux->prox[indice];
	}

	if (palavra[0] >= 48 && palavra[0] <= 57){
		if (tam >= 2 && palavra[1] == 'x')
			return novoToken(palavra, Hexadecimal, linha);
		else
			return novoToken(palavra, Decimal, linha);
	}

	if (aux->forma_palavra && aux->token != NULL){
		return novoToken(aux->token->palavra, aux->token->tipo, linha);
	}

	return NULL;
}



void imprimirTrie(TRIENODE *raiz){
	for (int i = 0; i < ALFABETO_TAM; i++)
		if (raiz->prox[i] != NULL)
			imprimirTrie(raiz->prox[i]);
		
	if (raiz->token && raiz->forma_palavra)
		printf ("token = %s, tipo = %d\n", raiz->token->palavra, (raiz->token->tipo - 1000));
}

void deletarTrie (TRIENODE *raiz) {
	TRIENODE *aux;

	for (int i = 0; i < ALFABETO_TAM; i++)
		if (raiz->prox[i] != NULL){
			deletarTrie(raiz->prox[i]);
			aux = raiz->prox[i];
			if (aux->token){
				if (aux->token->palavra){
					free(aux->token->palavra);
				}
				free (aux->token);
			}
			free (aux);
		}
}



