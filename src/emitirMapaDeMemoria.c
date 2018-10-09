#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAPA_TAM 1024
#define LINHA_TAM 5

/*
    ---- Voce deve implementar essa função para a Parte 2! ----
    Utilize os tokens da estrutura de tokens para montar seu código!
    Retorna:
        *  1 caso haja erro na montagem; (imprima o erro em stderr)
        *  0 caso não haja erro.
 */


char *traduz_instr(Token tok){
	if (!strcmp(tok.palavra, "ld") ) {

	} 
	else if () {

	}
	else if {

	}
}




int emitirMapaDeMemoria()
{
    int i = 0;
    int qtd_tokens = getNumberOfTokens();
    char ***mapa = NULL;

    mapa = malloc (MAPA_TAM * sizeof(char**));
    for (int i = 0; i <MAPA_TAM; i++){
    	mapa[i] = malloc(LINHA_TAM * sizeof(char*));
    	for (int j = 0; j < LINHA_TAM; j++) {
    		mapa[i][j] = NULL;
    	} 
    }


    Token aux;
    for (i = 0; i < qtd_tokens; i++) {
    	aux = recuperaToken(i);
    }


    return 0;
}
