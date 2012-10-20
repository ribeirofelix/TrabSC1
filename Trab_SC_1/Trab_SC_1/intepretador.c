#include <stdio.h>
#include <stdlib.h>
#include "interpretador.h"


typedef struct _leitor{
	int qtdComandos;
	char** vetComandos;
	int comandoAtual;
} Leitor;


pLeitor criarLeitor()
{
	pLeitor leitorRet = (pLeitor) malloc(sizeof(Leitor));
	if(leitorRet != NULL)
		return leitorRet;
	return NULL;
}

void inicializaLeitor(pLeitor pLeitor)
{
	pLeitor->qtdComandos = 0;
	pLeitor->vetComandos = NULL;
	pLeitor->comandoAtual = 0 ;
	return;
}

int getComandoAtual(pLeitor pleitor)
{
	return pleitor->comandoAtual;
}

int getQtdComando(pLeitor pleitor)
{
	return pleitor->qtdComandos;
}
