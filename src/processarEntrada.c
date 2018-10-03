#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"

/*
    Exemplo de erros:
    const char* get_error_string (enum errors code) {
        switch (code) {
            case ERR_HEXADECIMAL_NAO_VALIDO:
                return "LEXICO: Numero Hexadecimal Inválido!";
            case ERRO_ROTULO_JA_DEFINIDO:
                return "GRAMATICAL: ROTULO JA FOI DEFINIDO!";
            case ERR_DIRETIVA_NAO_VALIDA:
                return "LEXICO: Diretiva não válida";
*/

/*
    ---- Você Deve implementar esta função para a parte 1.  ----
    Essa função é chamada pela main em main.c
    Entrada da função: arquivo de texto lido e seu tamanho
    Retorna:
        * 1 caso haja erro na montagem; (imprima o erro em stderr)
        * 0 caso não haja erro.         (Caso não haja erro, na parte 1, ao retornar desta função, a lista de Tokens (adicionados utilizando a função adicionarToken()) é impressa)
*/

char *Buffer (char input){
    static char temp[4097];
    static int indice = 0;
    static int lendo_vazio = 0;
    static int lendo_coment = 0;
    char *resposta = NULL;
    
    if (lendo_vazio) {
    	if (input == ' ' || input == '\t' || input == '\n')
    		return NULL;
    	else if (input == '#')
    		lendo_coment = 1;

    	lendo_vazio = 0;       
    }

    if (lendo_coment) {
    	if (input == '\n' || input == '\0'){
    		lendo_coment = 0;
    		lendo_vazio = 1;
    	}
    	return NULL;
    } 

    if (input != ' ' && input != '\t' && input != '\n' && input != '\0' && input != '#'){
        temp[indice++] = tolower(input);
        return NULL;
	} else {
    	temp[indice++] = '\0';
    	resposta = malloc (indice * sizeof(char));
    	strcpy (resposta, temp);
    	indice = 0;
    	
        if (input == '#') {
			lendo_coment = 1;
			lendo_vazio = 0;
	    } else {
			lendo_vazio = 1;
	    }
	   
        return resposta;
    }

    

    return NULL;
}

NODE *inicializarTrie() {
    static int trie_inicializada = 0;
    
    if (!trie_inicializada){
        NODE *raiz = malloc(sizeof(NODE));
        raiz->nome = NULL;
        raiz->token = NULL;

        for (int j = 0; j < ALFABETO_TAM; j++)
                raiz->prox[j] = NULL;

        char elementos[][2][20] = {
            { {"Diretiva"}, {".set"}, },
            { {"Diretiva"}, {".org"}, },
            { {"Diretiva"}, {".align"}, },
            { {"Diretiva"}, {".wfill"}, },
            { {"Diretiva"}, {".word"}, },
            { {"Instrucao"}, {"ld"}, },
            { {"Instrucao"}, {"ldinv"}, },
            { {"Instrucao"}, {"ldabs"}, },
            { {"Instrucao"}, {"ldmq"}, },
            { {"Instrucao"}, {"ldmqmx"}, },
            { {"Instrucao"}, {"store"}, },
            { {"Instrucao"}, {"jump"}, },
            { {"Instrucao"}, {"jumpl"}, },
            { {"Instrucao"}, {"jumpr"}, },
            { {"Instrucao"}, {"add"}, },
            { {"Instrucao"}, {"addabs"}, },
            { {"Instrucao"}, {"sub"}, },
            { {"Instrucao"}, {"subabs"}, },
            { {"Instrucao"}, {"mult"}, },
            { {"Instrucao"}, {"div"}, },
            { {"Instrucao"}, {"lsh"}, },
            { {"Instrucao"}, {"rsh"}, },
            { {"Instrucao"}, {"storal"}, },
            { {"Instrucao"}, {"storar"}, },
            { {"Hexadecimal"}, {"0x0"}, },
            { {"Decimal"}, {"0"}, },
        };

        for (int i = 0; i < 26; i++){
            if (!strcmp(elementos[i][0], "Instrucao"))
                inserirTrie(raiz, Instrucao, elementos[i][1]);
            else if (!strcmp(elementos[i][0], "Diretiva"))
                inserirTrie(raiz, Diretiva, elementos[i][1]);
            else if (!strcmp(elementos[i][0], "Hexadecimal"))
                inserirTrie(raiz, Hexadecimal, elementos[i][1]);
            else if (!strcmp(elementos[i][0], "Decimal"))
                inserirTrie(raiz, Decimal, elementos[i][1]);
        }

        trie_inicializada = 1;
        return raiz;
    }

   return NULL;
}

Token *novoToken (char *palavra, TipoDoToken tipo, unsigned linha){
    int tam = strlen(palavra);
    
    Token *temp = malloc(sizeof(Token));
    
    temp->palavra = malloc((tam+1)*sizeof(char));
    strcpy(temp->palavra, palavra);

    temp->tipo = tipo;
    temp->linha = linha;

    return temp;
}
/*
int checarNumero(char *palavra){
    int tam = strlen(palavra);
    int hex = 0;
    int indice = 0;
    if (tam ==2 && palavra[0] == '0' && palavra[1] == 'x'){
        return -1;
    } else if (tam > 2 && palavra[0] == '0' && palavra[1] == 'x') {
        hex = 1;
        indice = 3;
    }
    

    for (; indice < tam; indice++){
        if (palavra[0] >= 48 && palavra[0] <= 57)
    }
}
*/
//retorna 1 se for Hexadecimal, 0 se não for
int checarHex(char *hex){
    int tam = strlen(hex);

    if (tam <= 2)
        return 0;
    else
        for (int i = 2; i < tam; i++)
            if (!((hex[i] >= 48 && hex[i] <= 57) || (hex[i] >= 97 && hex[i] <= 102) || (hex[i] >= 65 && hex[i] <= 70))) //checa se o char não esta entre 0-9, a-f, A-F
                return 0;

    return 1;
}
//retorna 1 se for Decimal, 0 se não for
int checarDec(char *dec){
    int tam = strlen(dec);

    for (int i = 0; i < tam; i++)
        if (!(dec[i] >= 48 && dec[i] <= 57))
            return 0;

    return 1;
}



Token *classificarPalavra(NODE *raiz, char *palavra, unsigned linha) {
    Token *aux = NULL;
    int tam = strlen(palavra);
    if (tam == 0){
        printf("Palavra Vazia\n");
        return NULL; //Erro: Palavra Vazia
    }

    if ( (aux = buscarTrie(raiz, palavra, linha)) ){ //palavra é Diretiva, Instrucao, Hexadecimal ou Decimal
        if (aux->tipo == Hexadecimal){
            if (checarHex(aux->palavra))
                return aux;
            else
                return NULL; //ERRO Léxico: Hexadecimal escrito errado
        } else if (aux->tipo == Decimal) {
            if (checarDec(aux->palavra))
                return aux;
            else
                return NULL; //ERRO Léxico: Decimal escrito errado
        } else {
            return aux; //Palavra é Diretiva ou Instrucao
        } 
    } else { //Palavra é Nome ou DefRotulo
        if (palavra[tam-1] == ':'){
            for (int i = 0; i < tam-1; i++){
                if (palavra[i] == ':')
                    return NULL; // Erro Lexico: ':' no meio de um nome
            }
            return novoToken (palavra, DefRotulo, linha);
        } else {
            for (int i = 0; i < tam; i++){
                if (palavra[i] == ':')
                    return NULL; // Erro Lexico: ':' no meio de um nome
            }
            return novoToken (palavra, Nome, linha);
        }
    }
    return NULL; //Codigo não deveria chegar aqui
}


int processarEntrada(char* entrada, unsigned tamanho)
{

    NODE *raiz = inicializarTrie();

   
    //printf ("%s\n", entrada);
	char *aux;
    int linha_atual = 1;
    //Token linha[10];
    Token *temp;
    for (int i = 0; entrada[i] != '\0'; i++){
        if ( (aux = Buffer(entrada[i])) ){
            temp = classificarPalavra(raiz, aux, linha_atual);
            adicionarToken (*temp);
        }
        
        

        if (entrada[i] == '\n')
            linha_atual++;
    }



/*
    Token *aux = NULL;
    char bus[50];
    strcpy (bus, "0:");
    aux = buscarTrie(raiz, bus, 0);
    if (aux)
        printf ("busca (%s): palavra = %s, tipo = %d\n", bus, aux->palavra, aux->tipo);
    else
        printf ("busca não encontrou (%s)\n", bus);
*/


    //printf("inseriu\n");

    //imprimirTrie (raiz);

    return 0;
}
