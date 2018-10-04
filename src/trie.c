#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"


//a-z (0-25); . (26); _ (27); 0-9 (28)
#define ALFABETO_TAM 29 //(a-z + _ + . + (0-9) ) 

int checarNome (Token *tok);
int checarInstrucao (Token *tok);
int checarDiretiva (Token *tok);
int checarDec1_1023 (Token *tok);
int checarHexDec0_1023 (Token *tok);
int checarHexDec0_MAX (Token *tok);
int checarHexDecMIN_MAX (Token *tok);
int checarVazio (Token *tok);


typedef struct ARG{
	int qtd_arg;
	int tam_vet1;
	int tam_vet2;

	int (*vet1[10])(Token* x);
	int (*vet2[10])(Token* x);

} ARG;


typedef struct TRIENODE{
	struct TRIENODE *prox[ALFABETO_TAM];
	int forma_palavra;
	Token *token;
	ARG *arg;
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

ARG *novoARG(char *palavra){
	ARG *aux = malloc (sizeof(ARG));
	aux->qtd_arg = 1;
	aux->tam_vet1 = 0;
	aux->tam_vet2 = 0;

	if (strcmp(palavra, ".set")){
		aux->qtd_arg = 2;
		aux->vet1[0] = &checarNome;
		aux->tam_vet1 = 1;
		aux->vet2[0] = &checarHexDec0_MAX;
		aux->tam_vet2 = 1;
	}
	else if (strcmp(palavra, ".org")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->tam_vet1 = 1;
	}
	else if (strcmp(palavra, ".align")){
		aux->vet1[0] = &checarDec1_1023;
		aux->tam_vet1 = 1;
	}
	else if (strcmp(palavra, ".wfill")){
		aux->qtd_arg = 2;
		aux->vet1[0] = &checarDec1_1023;
		aux->tam_vet1 = 1;
		aux->vet2[0] = &checarHexDecMIN_MAX;
		aux->vet2[1] = &checarNome;
		aux->tam_vet2 = 2;
	}
	else if (strcmp(palavra, ".word")){
		aux->vet1[0] = &checarHexDecMIN_MAX;
		aux->vet1[1] = &checarNome;
		aux->tam_vet2 = 2;
	}
	else if (strcmp(palavra, "ld")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "ldinv")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "ldabs")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "ldmq")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "ldmqmx")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "store")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "jump")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "jumpl")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "jumpr")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "add")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "addabs")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "sub")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "subabs")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "mult")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "div")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "lsh")){
		aux->vet1[0] = &checarVazio;
		aux->tam_vet1 = 1;
	}
	else if (strcmp(palavra, "rsh")){
		aux->vet1[0] = &checarVazio;
		aux->tam_vet1 = 1;
	}
	else if (strcmp(palavra, "storal")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	}
	else if (strcmp(palavra, "storar")){
		aux->vet1[0] = &checarHexDec0_1023;
		aux->vet1[1] = &checarNome;
		aux->tam_vet1 = 2;
	} else {
		return NULL;
	}

	return aux;
}

static Token *novoToken (char *palavra, TipoDoToken tipo, unsigned linha){
    int tam = strlen(palavra);
    
    Token *temp = malloc(sizeof(Token));
    
    temp->palavra = malloc((tam+1)*sizeof(char));
    strcpy(temp->palavra, palavra);

    temp->tipo = tipo;
    temp->linha = linha;

    return temp;
}

void inserirTrie (TRIENODE *raiz, char *palavra, Token *token, ARG *arg){
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
	aux->token = token;
	aux->arg = arg;
}

Token *buscarTrieToken (TRIENODE *raiz, char *palavra, unsigned linha){
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

ARG *buscarTrieArg (TRIENODE *raiz, Token *token){
	ARG *arg = malloc (sizeof(ARG));
	arg->qtd_arg = 1;
	arg->tam_vet1 = 0;
	arg->tam_vet2 = 0;

	if (token->tipo == Hexadecimal || token->tipo == Decimal || token->tipo == Nome){
		arg->vet1[0] = &checarVazio;
		arg->tam_vet1 = 1;
		return arg;
	} else if (token->tipo == DefRotulo) {
		arg->vet1[0] = &checarDiretiva;
		arg->vet1[1] = &checarInstrucao;
		arg->tam_vet1 = 1;
		return arg;
	}

	char *palavra = token->palavra;
	int tam = strlen(palavra);
	int indice;

	

	TRIENODE *aux = raiz;

	for (int i = 0; i < tam; i++){
		indice = getIndice(palavra[i]);
		if (indice < 0)
			break;

		if (!aux->prox[indice]){			
			return NULL;
		}

		aux = aux->prox[indice];
	}

	if (aux->forma_palavra && aux->arg != NULL){
		return aux->arg;
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


int checarNome (Token *tok){
	if (tok && tok->tipo == Nome)
		return 0;
	return 1;
}

int checarInstrucao (Token *tok){
	if (tok && tok->tipo == Instrucao)
		return 0;
	return 1;
}

int checarDiretiva (Token *tok){
	printf("%s %d", tok->palavra, tok->tipo);
	if (tok && tok->tipo == Diretiva)
		return 0;
	return 1;
}


int checarDec1_1023 (Token *tok){
	if (tok && tok->tipo != Decimal)
		return 1;
	char *p;
	long int valor;
	valor = strtol (tok->palavra, &p, 0);
	if (valor >= 1 && valor <= 1023)
		return 0;
	return 1;
}

int checarHexDec0_1023 (Token *tok){
	if (tok && tok->tipo != Decimal && tok->tipo != Hexadecimal)
		return 1;
	char *p;
	long int valor;
	valor = strtol (tok->palavra, &p, 0);
	if (valor >= 0 && valor <= 1023)
		return 0;
	return 1;
}

int checarHexDec0_MAX (Token *tok){
	if (tok && tok->tipo != Decimal && tok->tipo != Hexadecimal)
		return 1;
	char *p;
	long int valor;
	valor = strtol (tok->palavra, &p, 0);
	if (valor >= 0 && valor <= 2147483647)
		return 0;
	return 1;
}

int checarHexDecMIN_MAX (Token *tok){
	if (tok && tok->tipo != Decimal && tok->tipo != Hexadecimal)
		return 1;
	char *p;
	long int valor;
	valor = strtol (tok->palavra, &p, 0);
	if (valor >= -2147483648 && valor <= 2147483647)
		return 0;
	return 1;
}

int checarVazio (Token *tok){
	return 1;
}


