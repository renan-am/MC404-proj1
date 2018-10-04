#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"
//#include "func_aux.c"

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
    static int lendo_palavra = 0;
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
        lendo_palavra = 1;
        temp[indice++] = tolower(input);
        return NULL;
	} else if (lendo_palavra) {
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
	   	lendo_palavra = 0;
        return resposta;
    }

    if (input == '#') {
			lendo_coment = 1;
			lendo_vazio = 0;
	    } else {
			lendo_vazio = 1;
	    }
	   	lendo_palavra = 0;
    

    return NULL;
}

TRIENODE *inicializarTrie() {
    static int trie_inicializada = 0;
    
    if (!trie_inicializada){
        TRIENODE *raiz = malloc(sizeof(TRIENODE));
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

Token *classificarPalavra(TRIENODE *raiz, char *palavra, unsigned linha) {
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
            else{
                fprintf(stderr, "ERRO LEXICO: palavra inválida na linha %d!\n", linha);
                return NULL; //ERRO Léxico: Hexadecimal escrito errado
            }
        } else if (aux->tipo == Decimal) {
            if (checarDec(aux->palavra)){
                return aux;
            } else{
                fprintf(stderr, "ERRO LEXICO: palavra inválida na linha %d!\n", linha);
                return NULL; //ERRO Léxico: Decimal escrito errado
            }
        } else {
            return aux; //Palavra é Diretiva ou Instrucao
        } 
    } else { //Palavra é Nome ou DefRotulo
        if (palavra[tam-1] == ':'){
        	if (palavra[0] == '.'){
	            fprintf(stderr, "ERRO LEXICO: palavra inválida na linha %d!\n", linha);
	            return NULL; // Erro Lexico: '.' no inicio de um nome
	    	}
            for (int i = 0; i < tam-1; i++){
                if (
                    !(
                        (palavra[i] >= 48 && palavra[i] <= 57)  || 
                        (palavra[i] >= 97 && palavra[i] <= 122) || 
                        (palavra[i] >= 65 && palavra[i] <= 90)  || 
                        (palavra[i] == '_') 
                    )
                ){
                    fprintf(stderr, "ERRO LEXICO: palavra inválida na linha %d!\n", linha);
                    return NULL; // Erro Lexico: Símbolo invalido no meio de um nome
            	}
            }
            return novoToken (palavra, DefRotulo, linha);
        } else {
        	if (palavra[0] == '.'){
	            fprintf(stderr, "ERRO LEXICO: palavra inválida na linha %d!\n", linha);
	            return NULL; // Erro Lexico: '.' no começo de um nome
	    	}
            for (int i = 0; i < tam; i++){
                if (
                    !(
                        (palavra[i] >= 48 && palavra[i] <= 57)  || 
                        (palavra[i] >= 97 && palavra[i] <= 122) || 
                        (palavra[i] >= 65 && palavra[i] <= 90)  || 
                        (palavra[i] == '_') 
                    )
                ){
                    fprintf(stderr, "ERRO LEXICO: palavra inválida na linha %d!\n", linha);
                    return NULL; // Erro Lexico: Símbolo invalido no meio de um nome
                }
            }
            return novoToken (palavra, Nome, linha);
        }
    }
    fprintf(stderr, "ERRO LEXICO: palavra inválida na linha %d!\n", linha);
    return NULL; //Codigo não deveria chegar aqui
}


//return 1 = erro
//return 0 = OK
int checarErroGram(Token **linha, int tam, int fim){
	
	for (int i = 0; i < tam; i++){
		if (linha[i]->tipo == Instrucao){
			if ( !strcmp(linha[i]->palavra, "lsh") || !strcmp(linha[i]->palavra, "rsh") || !strcmp(linha[i]->palavra, "ldmq") ){
				if (fim){
					if (i+1 >= tam)
						return 0;
					else
						return 1; //ERRO: não pode ter mais tokens nessa linha
				}
				else
					return 0;
			} else {
				if (i+1 >= tam){
					if (fim)
						return 1; //instrução no fim da linha sem endereço seguindo
					else
						return 0;
				} else {
					if ( checarNome(linha[i+1]) && checarHexDec0_1023(linha[i+1]) ){
						return 1;
					} else {
						if (fim){
							if (i+2 >= tam)
								return 0;
							else
								return 1; //ERRO: Não pode ter mais tokens nessa linha
						} else {
							return 0;
						}
					}
				} 
			}
		}
		else if (linha[i]->tipo == Diretiva){
			if ( !strcmp(linha[i]->palavra, ".wfill") ){
				if (i+1 >= tam){
					if (fim)
						return 1; //ERRO .wfill no fim da linha
					else 
						return 0;	
				} else {
					if ( checarDec1_1023(linha[i+1]) )
						return 1; //ERRO: argumento depois de .wfill não é decimal entre 1 e 1023
					else {
						if (i+2 >= tam){
							if (fim)
								return 1; //ERRO .wfill sem o segundo o argumento
							else 
								return 0;	
						} else {
							if ( checarHexDecMIN_MAX (linha[i+2]) && checarNome (linha[i+2]))
								return 1; //ERRO: 2º argumento de .wfill errado
							else {
								if (fim){
									if (i+3 >= tam)
										return 0;
									else
										return 1; //ERRO: Não pode ter mais tokens nessa linha
								} else {
									return 0;
								}
							}
						}
					}		
				}
			}
			else if (!strcmp(linha[i]->palavra, ".set")){
				if (i+1 >= tam){
					if (fim)
						return 1; //ERRO .set no fim da linha
					else 
						return 0;	
				} else {
					if ( checarNome (linha[i+1]) ){
						return 1; //ERRO: argumento depois de .set não é nome
					}
					else {
						if (i+2 >= tam){
							if (fim)
								return 1; //ERRO .set sem o segundo o argumento
							else 
								return 0;	
						} else {
							if ( checarHexDec0_MAX (linha[i+2]))
								return 1; //ERRO: 2º argumento de .set errado
							else {
								if (fim){
									if (i+3 >= tam)
										return 0;
									else
										return 1; //ERRO: Não pode ter mais tokens nessa linha
								} else {
									return 0;
								} 
							}
						}
					}		
				}
			}
			else if (!strcmp(linha[i]->palavra, ".org")){
				if (i+1 >= tam){
					if (fim)
						return 1; // falta argumento
					else
						return 0;
				} else {
					if ( checarNome(linha[i+1]) && checarHexDec0_1023(linha[i+1]) ){
						return 1;
					} else {
						if (fim){
							if (i+2 >= tam)
								return 0;
							else
								return 1; //ERRO: Não pode ter mais tokens nessa linha
						} else {
							return 0;
						} 
					}
				} 
			}
			else if (!strcmp(linha[i]->palavra, ".align")){
				if (i+1 >= tam){
					if (fim)
						return 1; // falta argumento
					else
						return 0;
				} else {
					if ( checarDec1_1023(linha[i+1]) ){
						return 1;
					} else {
						if (fim){
							if (i+2 >= tam)
								return 0;
							else
								return 1; //ERRO: Não pode ter mais tokens nessa linha
						} else {
							return 0;
						}
					}
				} 
			}
			else if (!strcmp(linha[i]->palavra, ".word")){
				if (i+1 >= tam){
					if (fim)
						return 1; // falta argumento
					else
						return 0;
				} else {
					if ( checarHexDecMIN_MAX (linha[i+1]) && checarNome (linha[i+1]) ){
						return 1;
					} else {
						if (fim){
							if (i+2 >= tam)
								return 0;
							else
								return 1; //ERRO: Não pode ter mais tokens nessa linha
						} else {
							return 0; 
						}
					}
				}
			}
		}
		else if (linha[i]->tipo == DefRotulo){
			if (i+1 < tam) {
				if (checarNome (linha[i+1]) || checarInstrucao(linha[i+1]) || checarDiretiva(linha[i+1])){
					continue;
				} else {
					return 1; // token depois de DefRotulo invalido
				}
			} else {
				return 0;
			}
		}
		else if ((linha[i]->tipo == Nome || linha[i]->tipo == Decimal || linha[i]->tipo == Hexadecimal) && i == 0) {
			return 1; //Esses tokens não podem estar no inicio de uma frase
		}
	}
	return 0;
}

int processarEntrada(char* entrada, unsigned tamanho){

    TRIENODE *raiz = inicializarTrie();

   	char *aux;
    int linha_atual = 1;
    Token *temp;
    Token *linha_temp[10];
    int indice = 0;

    for (int i = 0; entrada[i] != '\0'; i++){
        if ( (aux = Buffer(entrada[i])) ){
            temp = classificarPalavra(raiz, aux, linha_atual);
            if (!temp)
            	return 1;
            else {
            	linha_temp[indice++] = temp;
            	if (checarErroGram(linha_temp, indice, 0)){
            		fprintf(stderr, "ERRO GRAMATICAL: palavra na linha %d!\n", linha_atual);
            		return 1;
            	}
            }
        }
	    if (entrada[i] == '\n'){
	        if (checarErroGram(linha_temp, indice, 1)){
				fprintf(stderr, "ERRO GRAMATICAL: palavra na linha %d!\n", linha_atual);
				return 1;
			} else {
				for (int j = 0; j < indice; j++){
					adicionarToken (*linha_temp[j]);
				}
			}
			indice = 0;
	        linha_atual++; 
		}
	}
	return 0;
}
