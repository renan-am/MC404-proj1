#ifndef TRIE_H
#define TRIE_H

//a-z (0-25); . (26); _ (27) 

#define ALFABETO_TAM 28 //(a-z + _ + .) 


typedef struct NODE{
	char *nome;
	struct NODE *prox[ALFABETO_TAM];
	Token *token;
} NODE;


#endif  