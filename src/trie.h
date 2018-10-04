#ifndef TRIE_H
#define TRIE_H

//a-z (0-25); . (26); _ (27) 
#define ALFABETO_TAM 28 //(a-z + _ + .)


typedef struct ARG{
	int qtd_arg;
	int tam_vet1;
	int tam_vet2;

	int (*vet1[5])(Token* x);

} ARG;

typedef struct TRIENODE{
	char *nome;
	struct TRIENODE *prox[ALFABETO_TAM];
	Token *token;
	ARG *arg;
} TRIENODE;


ARG *novoARG(char *palavra);
void inserirTrie (TRIENODE *raiz, char *palavra, Token *token, ARG *arg);

Token *buscarTrieToken(TRIENODE *raiz, char *palavra, unsigned linha);
ARG *buscarTrieArg (TRIENODE *raiz, Token *token);

void imprimirTrie(TRIENODE *raiz);

int checarNome (Token *tok);
int checarInstrucao (Token *tok);
int checarDiretiva (Token *tok);
int checarDec1_1023 (Token *tok);
int checarHexDec0_1023 (Token *tok);
int checarHexDec0_MAX (Token *tok);
int checarHexDecMIN_MAX (Token *tok);
int checarVazio (Token *tok);


#endif  