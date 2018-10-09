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

typedef struct DADOS {
    Token *tok;
    int POS;
    char ***mapa_rotulos;
} DADOS;

typedef struct MAP_ROT {
    char *nome;
    char *novo_valor;
    int lado; //-1: linha toda, 0: esquerda, 1: direita
    int tam_pos;
    int *pos_x;
    int *pox_y;
} MAP_ROT;


char *traduz_instr(Token tok){
	char *aux = malloc(5*sizeof(char));
    if ( !strcmp(tok.palavra, "ld") ) {
        strcpy (aux, "01");
        return aux;
	} 
	else if ( !strcmp(tok.palavra, "ldinv") ) {
        strcpy (aux, "02");
        return aux;
	}
	else if ( !strcmp(tok.palavra, "ldabs") ) {
        strcpy (aux, "03");
        return aux;
	}
    else if ( !strcmp(tok.palavra, "ldmq") ) {
        strcpy (aux, "0a");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "ldmqmx") ) {
        strcpy (aux, "09");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "store") ) {
        strcpy (aux, "21");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "jump") ) {
        strcpy (aux, "");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "jumpl") ) {
        strcpy (aux, "");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "jumpr") ) {
        strcpy (aux, "");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "add") ) {
        strcpy (aux, "05");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "addabs") ) {
        strcpy (aux, "07");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "sub") ) {
        strcpy (aux, "06");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "subabs") ) {
        strcpy (aux, "08");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "mult") ) {
        strcpy (aux, "0b");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "div") ) {
        strcpy (aux, "0c");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "lsh") ) {
        strcpy (aux, "14");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "rsh") ) {
        strcpy (aux, "15");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "storal") ) {
        strcpy (aux, "");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "storar") ) {
        strcpy (aux, "");
        return aux;
    }
}

char *traduz_endereco_HexDec(Token tok){
    char *aux = malloc (5*sizeof(char));
    long int temp = 0;
    char *lixo = NULL;

    temp = strtol(tok.palavra, &llixo, 0);
    sprintf(aux, "%03ld", i);
    return aux;
}

char *traduz_valor_HexDec(Token tok){
}


char **montar_linha(Token *tok, int pos){
    static Token linha_temp[LINHA_TAM];
    static int tam = 0;

    if (tok){
        linha[tam++] = *tok;
        return NULL;
    }

    char **nova_linha = malloc (LINHA_TAM * sizeof(char*));


    for (int i = 0; i < tam; i++){
        if (linha_temp[i].tipo == Instrucao){
            if (tam - i - 1 == 0){
                nova_linha[0] = traduz_instr(linha_temp[i]);
            }
            else if (tam - i - 1 == 1){
                nova_linha[0] = traduz_instr(linha_temp[i]);
                nova_linha[1] = traduz_endereco_HexDec(linha_temp[i]);
            }
        }
    }
}


int emitirMapaDeMemoria()
{
    int i = 0;
    int qtd_tokens = getNumberOfTokens();
    char ***mapa = NULL;

    mapa = malloc (MAPA_TAM * sizeof(char**));
    for (int i = 0; i <MAPA_TAM; i++){
    	mapa[i] = NULL;
    }


    Token aux;
    int linha = 0;
    int pos = 0;
    for (i = 0; i < qtd_tokens; i++) {
    	aux = recuperaToken(i);
        switch(aux.tipo){
            case Instrucao:
                mapa[linha][pos++] = traduz_instr(aux);
                break;
            case Diretiva:
                break;
            case Decimal:
            case Hexadecimal:
                mapa[linha][pos++] = traduz_endereco_HexDec(aux);
                break;
            case 
        }

        if (pos >= 5){
            linha++;
            pos = 0;
        }
    }


    return 0;
}
