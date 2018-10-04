#ifndef TRIE_H
#define TRIE_H

//a-z (0-25); . (26); _ (27) 
#define ALFABETO_TAM 28 //(a-z + _ + .)


typedef struct TRIENODE{
	char *nome;
	struct TRIENODE *prox[ALFABETO_TAM];
	Token *token;
} TRIENODE;

void inserirTrie (TRIENODE *raiz, TipoDoToken tipo, char *palavra);
Token *buscarTrie(TRIENODE *raiz, char *palavra, unsigned linha);
void imprimirTrie(TRIENODE *raiz);




#endif  