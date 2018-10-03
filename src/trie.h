#ifndef TRIE_H
#define TRIE_H

//a-z (0-25); . (26); _ (27) 
#define ALFABETO_TAM 28 //(a-z + _ + .)


typedef struct NODE{
	char *nome;
	struct NODE *prox[ALFABETO_TAM];
	Token *token;
} NODE;

void inserirTrie (NODE *raiz, TipoDoToken tipo, char *palavra);
Token *buscarTrie(NODE *raiz, char *palavra, unsigned linha);
void imprimirTrie(NODE *raiz);




#endif  