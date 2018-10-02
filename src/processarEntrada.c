#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        temp[indice++] = input;
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

int processarEntrada(char* entrada, unsigned tamanho)
{
    //printf ("%s\n", entrada);
	char *aux;
    for (int i = 0; entrada[i] != '\0'; i++){
    	if ( (aux = Buffer(entrada[i])) )
    		printf ("%s\n", aux);
    }


    return 0;
}
