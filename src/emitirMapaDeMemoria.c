//Aluno: Renan Clarindo Amorim
//RA: 186454

/*
    O código é estruturado em torno de 3 variaveis:
    1ª char ***mapa, é uma matriz 3d, onde as linhas são as linhas do código, e cada coluna representam o endereço, instruções e memórias (AAA | DD | DDD | DD | DDD)
    2ª MAPA_NOME *mapa_nome é um vetor de struct onde cada elemento repsenta um nome (tanto de rotulo como de .set). Em *nome é guardado o valor de referencia, em *endereco
    é guardado o valor que será substituido (endereço ou valor)
    3ª int *mapa_impressão é um mapa que representa os pulos feitos por causa do .org, permitindo que seja impresso corretamente o mapa de memoria (na ordem certa)

*/


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
    char *nome; //nome de referencia
    char *endereco; //valor que substitui o nome
    int lado; //-1: linha toda, 0: esquerda, 1: direita
    int tam_pos; // tamanho do vetor pos_lin e pos_col 
    // vetores que guardam a linha e coluna (da matriz mapa) de onde apareceram cada rotulo
    int *pos_lin;
    int *pos_col; 
} MAPA_NOME;

int busca_mapa_nome (MAPA_NOME *mapa_nome, int tam, char *nome);
void inseri_mapa_nome(MAPA_NOME *mapa_nome, int *tam, char *nome, long int endereco, int lado, int pos_lin, int pos_col, int set); 
char *traduz_instr(Token tok);
char *traduz_endereco_HexDec(Token tok);
char *traduz_valor_HexDec(Token tok);
int montar_linhas(char ***mapa, MAPA_NOME *mapa_nome, int *tam_mapa_nome, int mapa_impressao[]);
int substituirNomes(char ***mapa, MAPA_NOME *mapa_nome, int *tam_mapa_nome);
void imprimirMapa(char ***mapa, int mapa_impressao[]);


int emitirMapaDeMemoria()
{

    int mapa_impressao[1025]; //representa a sequencia de linhas para imprimir na ordem que o .org ocorre no código, na ultima posição, 1025, é guardado o valor onde deve iniciar a impressão do mapa de memória
    for (int i = 0; i < 1025; i++){
        mapa_impressao[i] = i+1;
    }
    mapa_impressao[1024] = 0;
    
    char ***mapa = NULL; //matriz 3d representado as linhas do codigo e seus elementos

    mapa = malloc (MAPA_TAM * sizeof(char**));
    for (int i = 0; i <MAPA_TAM; i++){
    	mapa[i] = malloc(5*sizeof(char*));
        for (int j = 1; j < 5; j++){
            mapa[i][j] = NULL;
        }
        mapa[i][0] = malloc(5*sizeof(char));
        sprintf(mapa[i][0], "%03X", i);
    }

    MAPA_NOME *mapa_nome = calloc (2000, sizeof(MAPA_NOME)); //vetor de struct que guarda os rotulos a medida que aparecem, também guarda a posição de onde cada um apareceu 
    for (int i = 0; i < 2000; i++){
        mapa_nome[i].nome = NULL;
        mapa_nome[i].endereco = 0;
        mapa_nome[i].pos_lin = malloc (2000* sizeof(int));
        mapa_nome[i].pos_col = malloc (2000* sizeof(int));
        mapa_nome[i].tam_pos = 0;
    }
    int tam_mapa_nome = 0;

    if (montar_linhas (mapa, mapa_nome, &tam_mapa_nome, mapa_impressao))
        return 1;

    if (substituirNomes(mapa, mapa_nome, &tam_mapa_nome))
        return 1;

    imprimirMapa (mapa, mapa_impressao);

    for (int i = 0; i < MAPA_TAM; i++){
        for (int j = 0; j<5; j++){
            if (mapa[i][j] != NULL) {
                free(mapa[i][j]);
            }
        }
        free(mapa[i]);
    }
    free(mapa);

    for (int i = 0; i < 2000; i++){
        if (mapa_nome[i].nome != NULL)
            free (mapa_nome[i].nome);
        if (mapa_nome[i].endereco != NULL) 
            free (mapa_nome[i].endereco);
        free (mapa_nome[i].pos_lin);
        free (mapa_nome[i].pos_col);
    }
    free (mapa_nome);
    return 0;
}


int busca_mapa_nome (MAPA_NOME *mapa_nome, int tam, char *nome) {
    for (int i = 0; i < tam; i++)
        if (!strcmp(mapa_nome[i].nome, nome))
            return i;

    return -1;
}

//Procura se o rotulo existe, e dependendo se é definição (DefRotulo: ou .set) ou não adiciona as informções pertinentes
void inseri_mapa_nome(MAPA_NOME *mapa_nome, int *tam, char *nome, long int endereco, int lado, int pos_lin, int pos_col, int set) {
    
    int fim = strlen(nome);
    if (nome[fim-1] == ':')
        nome[fim-1] = '\0';

    int indice = busca_mapa_nome(mapa_nome, *tam, nome);


    if (indice >= 0){
        if (endereco >= 0){
            char *temp = malloc (12*sizeof(char));
                
            if (set)
                sprintf(temp, "%lX", endereco);
            else
                sprintf(temp, "%03lX", endereco);
            
            mapa_nome[indice].endereco = temp;
            mapa_nome[indice].lado = lado;
        }
        else {
            int aux = mapa_nome[indice].tam_pos;
            mapa_nome[indice].pos_lin[aux] = pos_lin;
            mapa_nome[indice].pos_col[aux] = pos_col;
            (mapa_nome[indice].tam_pos)++;
        }
    } else {
        int aux = mapa_nome[*tam].tam_pos;
        int t_nome = strlen (nome);
        mapa_nome[*tam].nome = malloc ((t_nome+1)*sizeof(char));
        strcpy(mapa_nome[*tam].nome, nome);
        if (endereco >= 0){
            char *temp = malloc (12*sizeof(char));
            
            if (set)
                sprintf(temp, "%lX", endereco);
            else
                sprintf(temp, "%03lX", endereco);

            mapa_nome[*tam].endereco = temp;
            mapa_nome[*tam].lado = lado;
        } else {        
            mapa_nome[*tam].pos_lin[aux] = pos_lin;
            mapa_nome[*tam].pos_col[aux] = pos_col;
            mapa_nome[*tam].tam_pos++;
        }
        (*tam)++;
    }
}

//Retorna o valor hexadecimal
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
        strcpy (aux, "0A");
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
        strcpy (aux, "0D");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "jumpl") ) {
        strcpy (aux, "0F");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "jumpr") ) {
        strcpy (aux, "10");
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
        strcpy (aux, "0B");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "div") ) {
        strcpy (aux, "0C");
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
        strcpy (aux, "12");
        return aux;
    }
    else if ( !strcmp(tok.palavra, "storar") ) {
        strcpy (aux, "13");
        return aux;
    }

    return aux;
}

//Recebe um endereço decimal ou hexadecimal, e retorna uma string formatada
char *traduz_endereco_HexDec(Token tok){
    char *aux = malloc (5*sizeof(char));
    long int temp = 0;
    char *lixo = NULL;

    temp = strtol(tok.palavra, &lixo, 0);
    sprintf(aux, "%03lX", temp);
    return aux;
}

//Recebe um valor decimal ou hexadecimal, e retorna uma string formatada
char *traduz_valor_HexDec(Token tok){
    char *aux = malloc (12*sizeof(char));
    long int temp = 0;
    char *lixo = NULL;

    temp = strtol(tok.palavra, &lixo, 0);
    sprintf(aux, "%010lX", temp);
    return aux;
}

//Le a lista de tokens gerando as linha hexadecimais, também executa os comandos das diretivas e carrega em mapa_nome os Nomes que encontra
int montar_linhas(char ***mapa, MAPA_NOME *mapa_nome, int *tam_mapa_nome, int mapa_impressao[]){

    Token tok, tok1, tok2;
    int pos_lin = 0, pos_col = 0;
    char *lixo = NULL;
    
    int qtd_tokens = getNumberOfTokens();


    for (int i = 0; i < qtd_tokens; i++) {
        int lado = 0;
        if (pos_col == 0)
            pos_col = 1;
        
        tok = recuperaToken(i);

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
                    sprintf(mapa[pos_lin][pos_col], " ");
                    inseri_mapa_nome (mapa_nome, tam_mapa_nome, tok1.palavra, -1, -1, pos_lin, pos_col, 0);
                    pos_col++;
                } else {
                     mapa[pos_lin][pos_col++] = traduz_endereco_HexDec(tok1);
                }
            }
        }
        else if (tok.tipo == DefRotulo){
            if (pos_col == 3) //lado direito
                lado = 1;
            inseri_mapa_nome (mapa_nome, tam_mapa_nome, tok.palavra, pos_lin, lado, pos_lin, pos_col, 0);
        }
        else if (tok.tipo == Diretiva){
            if ( !strcmp(tok.palavra, ".set") ){
                tok1 = recuperaToken(++i);
                tok2 = recuperaToken(++i);
                long int end = strtol(tok2.palavra, &lixo, 0);
                inseri_mapa_nome (mapa_nome, tam_mapa_nome, tok1.palavra, end, -1, pos_lin, pos_col, 1);
            }
            else if ( !strcmp(tok.palavra, ".org") ){
                int temp = pos_lin;
                tok1 = recuperaToken(++i);
                pos_lin = strtol(tok1.palavra, &lixo, 0);
                
                //Garante que mapa_impressão é preenchido corretamente
                if (temp == 0 && pos_col == 1) //se o codigo começa com o org, o coloca em mapa_impressaõ[1024] que armazena por onde a impressão deve começar
                    mapa_impressao[1024] = pos_lin;
                if (temp != pos_lin) //evitar criar loops se o .org pular pra linha que já está 
                    mapa_impressao[temp] = pos_lin;
                if (pos_lin == 0) //Se pular de volta pra linha 0, garante que a proxima instrução a ser executada será a da linha 1
                    mapa_impressao[pos_lin] = 1;


                
                pos_col = 1;
                if (mapa[pos_lin][1] != NULL){
                    fprintf(stderr, "IMPOSSIVEL MONTAR CODIGO!\n");
                    return 1; //ERRO
                }
            }
            else if ( !strcmp(tok.palavra, ".word") ){
                if (pos_col != 1){
                    fprintf(stderr, "IMPOSSIVEL MONTAR CODIGO!\n");
                    return 1; //ERRO
                }
                
                tok1 = recuperaToken(++i);
                    if (tok1.tipo == Nome){
                        mapa[pos_lin][pos_col] = malloc(12*sizeof(char));
                        sprintf(mapa[pos_lin][pos_col], " ");
                        inseri_mapa_nome (mapa_nome, tam_mapa_nome, tok1.palavra, -1, -1, pos_lin, pos_col, 0);
                        pos_lin++;
                    } else {
                        mapa[pos_lin++][pos_col] = traduz_valor_HexDec(tok1);
                    }

            }
            else if ( !strcmp(tok.palavra, ".align") ){
                tok1 = recuperaToken(++i);
                long int val = strtol(tok1.palavra, &lixo, 0);
                while (pos_lin < MAPA_TAM){
                    if (pos_lin % val == 0){
                        if (pos_col == 3){//preenche o resto da linha com 0
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
                        if (pos_col == 3) {//preenche o resto da linha com 0
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
                if (pos_col != 1){
                    fprintf(stderr, "IMPOSSIVEL MONTAR CODIGO!\n");
                    return 1; //ERRO
                }
                tok1 = recuperaToken(++i);
                tok2 = recuperaToken(++i);
                long int tam = strtol(tok1.palavra, &lixo, 0);
                for (int k = 0; k < tam; k++){
                    if (mapa[pos_lin][pos_col] != NULL){
                        fprintf(stderr, "IMPOSSIVEL MONTAR CODIGO!\n");
                        return 1; //ERRO
                    }
                    if (tok2.tipo == Nome){
                        mapa[pos_lin][pos_col] = malloc(12*sizeof(char));
                        sprintf(mapa[pos_lin][pos_col], " ");
                        inseri_mapa_nome (mapa_nome, tam_mapa_nome, tok2.palavra, -1, -1, pos_lin, pos_col, 0);
                    } else {
                        mapa[pos_lin][pos_col] = traduz_valor_HexDec(tok2); 
                    }       
                    pos_lin++;
                }


            }
        }

        if (pos_col >= 5){
            pos_lin++;
            pos_col = 1;

            if (mapa[pos_lin][1] != NULL){
                fprintf(stderr, "IMPOSSIVEL MONTAR CODIGO!\n");
                return 1; //ERRO
            }
        }
    }
    if (pos_col == 3){
        mapa[pos_lin][pos_col] = malloc(5*sizeof(char));
        sprintf(mapa[pos_lin][pos_col], "00");
        pos_col++;
        mapa[pos_lin][pos_col] = malloc(5*sizeof(char));
        sprintf(mapa[pos_lin][pos_col], "000");
    }

    mapa_impressao[pos_lin] = 2000;
    return 0;
}

//Percorre mapa_nome e substitui eles pelos valores apropriados, também trata casos onde a instrução anterior depende da posição do Nome
int substituirNomes(char ***mapa, MAPA_NOME *mapa_nome, int *tam_mapa_nome) {
    MAPA_NOME *aux;
    for (int i = 0; i < *tam_mapa_nome; i++){
        aux = &mapa_nome[i];
        if (aux->endereco == NULL){
            fprintf(stderr, "ERRO: Usado mas não definido: %s\n", aux->nome);
            return 1; //ERRO
        }
        for (int j = 0; j < aux->tam_pos; j++){
            if (aux->lado == -1){
                const char *marg="00000000000000000000";
                int margem = 10 - strlen(aux->endereco); 
                if(margem < 0) 
                    margem = 0;
                sprintf(mapa[aux->pos_lin[j]][aux->pos_col[j]] ,"%*.*s%s", margem, margem, marg, aux->endereco); 
            } else { //Checa a instrução que vem antes se precisa de ajuste por causa da prosição do Nome
                sprintf(mapa[aux->pos_lin[j]][aux->pos_col[j]] ,"%s", aux->endereco);
                if ( !strcmp(mapa[aux->pos_lin[j]][aux->pos_col[j] - 1], "12") || !strcmp(mapa[aux->pos_lin[j]][aux->pos_col[j] - 1], "13" )){
                    if (aux->lado == 0)
                        sprintf(mapa[aux->pos_lin[j]][aux->pos_col[j] - 1] ,"12");
                    else 
                        sprintf(mapa[aux->pos_lin[j]][aux->pos_col[j] - 1] ,"13");
                } 
                else if ( !strcmp(mapa[aux->pos_lin[j]][aux->pos_col[j] - 1], "0F") || !strcmp(mapa[aux->pos_lin[j]][aux->pos_col[j] - 1], "10" )){
                    if (aux->lado == 0)
                        sprintf(mapa[aux->pos_lin[j]][aux->pos_col[j] - 1] ,"0F");
                    else 
                        sprintf(mapa[aux->pos_lin[j]][aux->pos_col[j] - 1] ,"10");
                }
                else if ( !strcmp(mapa[aux->pos_lin[j]][aux->pos_col[j] - 1], "0D") ){
                    if (aux->lado == 0)
                        sprintf(mapa[aux->pos_lin[j]][aux->pos_col[j] - 1] ,"0D");
                    else 
                        sprintf(mapa[aux->pos_lin[j]][aux->pos_col[j] - 1] ,"0E");
                }
            }
        }
    }

    return 0;
}

void imprimirMapa(char ***mapa, int mapa_impressao[]){
    for (int i = mapa_impressao[1024]; i < MAPA_TAM; i = mapa_impressao[i]){
        if (mapa[i][1] == NULL)
            continue;
        for (int j = 0; j < 5; j++){
            if (mapa[i][j] == NULL)
                continue;

            if (mapa[i][1] != NULL && mapa[i][2] == NULL){
                char aux[15];
                char aux1[4];
                char aux2[4];
                char aux3[4];
                char aux4[4];
                
                sprintf(aux, "%12s", mapa[i][1]);

                aux1[0] = mapa[i][1][0];
                aux1[1] = mapa[i][1][1];
                aux1[2] = '\0';

                aux2[0] = mapa[i][1][2];
                aux2[1] = mapa[i][1][3];
                aux2[2] = mapa[i][1][4];
                aux2[3] = '\0';
                
                aux3[0] = mapa[i][1][5];
                aux3[1] = mapa[i][1][6];
                aux3[2] = '\0';

                aux4[0] = mapa[i][1][7];
                aux4[1] = mapa[i][1][8];
                aux4[2] = mapa[i][1][9];
                aux4[3] = '\0';
                
                printf ("%s %s %s %s %s", mapa[i][0], aux1, aux2, aux3, aux4);
                break;
            } else {
                printf ("%s", mapa[i][j]);
                if (j + 1 != 5)
                    printf(" ");
            }
        } 
        printf("\n");
    }
}
