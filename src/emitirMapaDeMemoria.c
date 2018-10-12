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

typedef struct MAPA_NOME {
    char *nome;
    char *endereco;
    int lado; //-1: linha toda, 0: esquerda, 1: direita
    int tam_pos;
    int *pos_lin;
    int *pos_col;
    struct MAPA_NOME *prox;
} MAPA_NOME;

int busca_mapa_nome (MAPA_NOME *mapa_nome, int tam, char *nome) {

    for (int i = 0; i < tam; i++)
        if (!strcmp(mapa_nome[i]->nome, nome))
            return i;

    return -1;
}

void inseri_mapa_nome(MAPA_NOME *mapa_nome, int *tam, char *nome, long int endereco, int lado, int pos_lin, int pos_col, int set){
    int indice = busca_mapa_nome(mapa_nome, tam, nome);

    if (indice >= 0){
        if (endereco >= 0){
            char *temp = malloc (5*sizeof(char));
                
            if (set)
                sprintf(temp, "%x", endereco)
            else
                sprintf(temp, "%03x", endereco);
            
            mapa_nome[*tam].endereco = temp;
            mapa_nome[*tam].lado = lado;
        }
        else {
            int aux = mapa_nome[indice].tam_pos;
            mapa_nome[indice].pos_lin[aux] = pos_lin;
            mapa_nome[indice].pos_col[aux] = pos_col;
            mapa_nome[indice].tam_pos++;
        }
    } else {
        int aux = mapa_nome[*tam].tam_pos;
        int t_nome = strlen (nome)
        mapa_nome[*tam].nome = malloc ((t_nome+1)*sizeof(char));
        strcpy(mapa_nome[*tam].nome, nome);

        if (endereco >= 0){
            char *temp = malloc (5*sizeof(char));
            
            if (set)
                sprintf(temp, "%x", endereco)
            else
                sprintf(temp, "%03x", endereco);

            mapa_nome[*tam].endereco = temp;
            mapa_nome[*tam].lado = lado;
        } else {        
            mapa_nome[*tam].pos_lin[aux] = pos_lin;
            mapa_nome[*tam].pos_col[aux] = pos_col;
            (*tam)++;
        }
    }

}


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

    temp = strtol(tok.palavra, &lixo, 0);
    sprintf(aux, "%03x", i);
    return aux;
}

char *traduz_valor_HexDec(Token tok){
    char *aux = malloc (5*sizeof(char));
    long int temp = 0;
    char *lixo = NULL;

    temp = strtol(tok.palavra, &lixo, 0);
    sprintf(aux, "%010x", i);
    printf("valor convertido = %s", aux);
    return aux;
}



void montar_linhas(char ***mapa, MAPA_NOME *mapa_nome, int *tam_mapa_nome){

    Token tok, tok1, tok2;
    int qtd_tokens = getNumberOfTokens();

    char **nova_linha = malloc ( (tam+1) * sizeof(char*));

    int pos_lin = 0, pos_col = 0;

    for (i = 0; i < qtd_tokens; i++) {
        if (pos_col == 0)
            pos_col = 1;
        
        tok = recuperaToken(i);
        mapa[pos_lin][pos_col];

        if (tok.tipo == Instrucao){
            if ( !strcmp(tok.palavra, "lsh") || !strcmp(tok.palavra, "rsh") || !strcmp(tok.palavra, "ldmq") ) {             
                mapa[pos_lin][pos_col++] = traduz_instr(tok);
                mapa[pos_lin][pos_col] = malloc(5*sizeof(char));
                sprintf(mapa[pos_lin][pos_col], "000");
                pos_col++;
            }
            else {
                tok1 = recuperaToken(++i);
                mapa[pos_lin][pos_col++] = traduz_instr(tok);

                if (tok1.tipo == Nome){
                    mapa[pos_lin][pos_col] = malloc(5*sizeof(char));
                    sprintf(mapa[pos_lin][pos_col], "");
                    inseri_mapa_nome (mapa_nome, tam_mapa_nome, tok1.palavra, -1, -1, pos_lin, pos_col, 0);
                    pos_col++;
                } else {
                     mapa[pos_lin][pos_col++] = traduz_endereco_HexDec(tok1);
                }
            }
        }
        else if (tok.tipo == DefRotulo){
            int lado = 0;
            if (pos_col == 3) //lado direito
                lado = 1
            inseri_mapa_nome (mapa_nome, tam_mapa_nome, tok.palavra, pos_lin, lado, pos_lin, pos_col, 0);
        }
        else if (tok.tipo == Diretiva){
            if ( !strcmp(tok.palavra, ".set") ){
                tok1 = recuperaToken(++i);
                tok2 = recuperaToken(++i);
                char *lixo = NULL;
                long int end = strtol(tok2.palavra, &lixo, 0);
                inseri_mapa_nome (mapa_nome, tam_mapa_nome, tok1.palavra, end, lado, pos_lin, pos_col, 1);
            }
            else if ( !strcmp(tok.palavra, ".org") ){
                pos_lin = strtol(tok.palavra, &lixo, 0);
                pos_col = 1;
                if (mapa[pos_lin][1] != NULL)
                    return //ERRO
            }
            else if ( !strcmp(tok.palavra, ".word") ){
                if (pos_col != 1)
                    return //ERRO
                
                tok1 = recuperaToken(++i);
                    if (tok1.tipo == Nome){
                        mapa[pos_lin][pos_col] = malloc(12*sizeof(char));
                        sprintf(mapa[pos_lin][pos_col], "");
                        inseri_mapa_nome (mapa_nome, tam_mapa_nome, tok1.palavra, -1, -1, pos_lin, pos_col, 0);
                        pos_col++;
                    } else {
                        mapa[pos_lin][pos_col++] = traduz_valor_HexDec(tok1);
                    }

            }
            else if ( !strcmp(tok.palavra, ".align") ){
                tok1 = recuperaToken(++i);
                long int val = strtol(tok1.palavra, &lixo, 0);
                while (pos_lin < MAPA_TAM){
                    if (pos_lin % val == 0){
                        if (pos_col == 3){
                            mapa[pos_lin][pos_col] = malloc(5*sizeof(char));
                            sprintf(mapa[pos_lin][pos_col++], "00");
                            
                            mapa[pos_lin][pos_col] = malloc(5*sizeof(char));
                            sprintf(mapa[pos_lin][pos_col++], "000");
                            
                            pos_lin++;
                            pos_col = 1;
                        }
                        else
                            break;
                    } 
                    else {
                        if (pos_col == 3) {
                            mapa[pos_lin][pos_col] = malloc(5*sizeof(char));
                            sprintf(mapa[pos_lin][pos_col++], "00");
                            
                            mapa[pos_lin][pos_col] = malloc(5*sizeof(char));
                            sprintf(mapa[pos_lin][pos_col++], "000");
                        }
                        pos_lin++;
                        pos_col = 1;
                    }
                }
            }
            else if ( !strcmp(tok.palavra, ".wfill") ){
                if (pos_col != 1)
                    return; //ERRO
                tok1 = recuperaToken(++i);
                tok2 = recuperaToken(++i);
                char *lixo = NULL;
                long int tam = strtol(tok1.palavra, &lixo, 0);
                for (int k = 0; k < tam; k++){
                    if (tok2.tipo == Nome){
                        mapa[pos_lin][pos_col] = malloc(12*sizeof(char));
                        sprintf(mapa[pos_lin][pos_col], "");
                        inseri_mapa_nome (mapa_nome, tam_mapa_nome, tok1.palavra, -1, -1, pos_lin, pos_col, 0);
                    } else {
                        mapa[pos_lin][pos_col] = traduz_valor_HexDec(tok2); 
                    }   
                    pos_lin++;
                }


            }
            else if ( !strcmp(tok.palavra, "") ){

            }
        }





        if (pos_col >= 5){
            pos_lin++;
            pos_col = 1;

            if (mapa[pos_lin][1] != NULL)
                return //ERRO
        }
    }
}


int emitirMapaDeMemoria()
{
    int i = 0;
    char ***mapa = NULL;

    mapa = malloc (MAPA_TAM * sizeof(char**));
    for (int i = 0; i <MAPA_TAM; i++){
    	mapa[i] = malloc(5*sizeof(char*));
        for (int j = 1; j < 5; j++){
            mapa[i][j] = NULL;
        }
        mapa[i][0] = malloc(5*sizeof(char));
        sprintf(mapa[i][0], "%03x", i);
    }


    
    int pos_lin = 0;
    int pos = 0;
    
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
