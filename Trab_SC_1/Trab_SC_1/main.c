#include <stdio.h>
#include <stdlib.h>

#include "interpretador.h"
#include "escalonador.h"
int main(){
	
	pLeitor leitor = criarLeitor();
	pEscalonador pEscalonadorFifo = criaEscalonador( Fifo,0);
	pEscalonador pEscalonadorSJF = criaEscalonador(SJF,0);
	pEscalonador pEscalonadorPriopridades = criaEscalonador(Prioridades,0);
	pEscalonador pEscalonadorRoundRobin = criaEscalonador(RoudRobin,0);
	char * comando ;
	
	FILE * entrada = fopen("E:\\Documentos\\entrada.txt","rt");
	FILE * saida = fopen("E:\\Documento\\saida.txt","rt" );

	preencheCommandos(entrada , leitor);
	

	while ( (comando = interarComando(leitor) ) != NULL )
	{
		pProcesso pProcessoAtual = criaProcesso(0,0,0,0,0,0);
		insereProcesso(pEscalonadorFifo,pProcessoAtual);
	}

	executaEscalonamento(pEscalonadorFifo);
	
	return 0;
}