#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"


//return 1 = erro
//return 0 = ok

int checarNome (Token *tok){
	if (tok && tok->tipo == Nome)
		return 0;
	return 1;
}

int checarInstrucao (Token *tok){
	if (tok && tok->tipo == Instrucao)
		return 0;
	return 1;
}

int checarDiretiva (Token *tok){
	if (tok && tok->tipo == Diretiva)
		return 0;
	return 1;
}


int checarDec1_1023 (Token *tok){
	if (tok && tok->tipo != Decimal)
		return 1;
	char *p;
	long int valor;
	valor = strtol (tok->palavra, &p, 0);
	if (valor >= 1 && valor <= 1023)
		return 0;
	return 1;
}

int checarHexDec0_1023 (Token *tok){
	//printf ("tipo = %d\n", tok->tipo);
	if (tok && tok->tipo != Decimal && tok->tipo != Hexadecimal)
		return 1;
	char *p;
	long int valor;
	valor = strtol (tok->palavra, &p, 0);
	if (valor >= 0 && valor <= 1023)
		return 0;
	return 1;
}

int checarHexDec0_MAX (Token *tok){
	if (tok && tok->tipo != Decimal && tok->tipo != Hexadecimal)
		return 1;
	char *p;
	long int valor;
	valor = strtol (tok->palavra, &p, 0);
	if (valor >= 0 && valor <= 2147483647)
		return 0;
	return 1;
}

int checarHexDecMIN_MAX (Token *tok){
	if (tok && tok->tipo != Decimal && tok->tipo != Hexadecimal)
		return 1;
	char *p;
	long int valor;
	valor = strtol (tok->palavra, &p, 0);
	if (valor >= -2147483648 && valor <= 2147483647)
		return 0;
	return 1;
}

int checarVazio (Token *tok){
	return 1;
}

