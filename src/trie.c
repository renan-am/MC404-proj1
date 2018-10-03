#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

typedef struct NODE{
	struct NODE *prox[ALFABETO_TAM];
	int forma_palavra;
	Token *token;
} NODE;

int getIndice(char alvo){
	if (alvo >= 97 && alvo <= 122)
		return (alvo - 'a');
	else if (alvo >= 65 && alvo <= 90)
		return (alvo + 40 - 'A');
	else if (alvo == '.')
		return 27;
	else if (alvo == '_')
		return 28;
	else
		return -1;
}

struct TrieNode *getNode() 
{ 
    NODE *aux = NULL; 
  
    aux = malloc(sizeof(NODE)); 
  
    if (aux) 
    { 
        int i; 
  
        for (i = 0; i < ALPHABET_SIZE; i++) 
            pNode->children[i] = NULL; 
    } 
  
    return pNode; 
}

void inserir (NODE *raiz, char *palavra, TipoDoToken tipo){
	int tam = strlen(palavra);
	int indice;

	NODE *aux = raiz;

	for (int i = 0; i < tam; i++){
		indice = getIndice(palavra[i]);
		
		if (!aux->prox[indice]){
			NODE *temp = malloc (sizeof(NODE));
			temp->forma_palavra = 0;
			temp->token = NULL;
			
			for (int j = 0; j < ALPHABET_TAM; j++)
				temp->prox[j] = NULL;

			aux->prox[indice] = temp;
		}

		aux = aux->prox[indice];
	}

	aux->forma_palavra = 0;
	aux->token = malloc(sizeof(Token));
	aux->token->palavra = malloc(tam*sizeof(char));
	strcpy (aux->token->palavra, palavra);
	aux->token->tipo = tipo;
}



void imprimirTrie(NODE *raiz){
	
}



