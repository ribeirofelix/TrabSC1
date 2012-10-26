#include <stdio.h>
#include <stdlib.h>

//#include "interpretador.h"
#include "escalonador.h"
int main(){
	
	//pLeitor leitor = criarLeitor();
	pEscalonador pEscalonadorFifo = criaEscalonador( Fifo,0);
	pEscalonador pEscalonadorSJF = criaEscalonador(SJF,0);
	pEscalonador pEscalonadorPriopridades = criaEscalonador(Prioridades,0);
	pEscalonador pEscalonadorRoundRobin = criaEscalonador(RoudRobin,0);
	pProcesso pProcessoAInserir  ;
	int comando = 5 ;
	FILE * entrada = fopen("E:\\Documentos\\entrada.txt","rt");
	FILE * saida = fopen("E:\\Documento\\saida.txt","rt" );

//	preencheCommandos(entrada , leitor);
	

	/*while ( (pProcessoAInserir = interarComando(leitor) ) != NULL )
	{
		insereProcesso(pEscalonadorFifo,pProcessoAInserir);
	}*/

	while (comando != 0)
	{
		pProcesso pProcesso = NULL;
		if(comando%2)
			 pProcesso = criaProcesso(0,0,comando,0,comando,comando,"ls");
		else
			 pProcesso = criaProcesso(0,0,comando,0,comando,comando,"man");
		insereProcesso(pEscalonadorFifo,pProcesso);
		comando--;
	}
	comando = 5 ;
	while (comando != 0)
	{
		pProcesso pProcesso = NULL;
		if(comando%2)
			 pProcesso = criaProcesso(0,0,comando,0,comando,comando,"ls");
		else
			 pProcesso = criaProcesso(0,0,comando,0,comando,comando,"man");
		insereProcesso(pEscalonadorSJF,pProcesso);
		comando--;
	}

		comando = 5 ;
	while (comando != 0)
	{
		pProcesso pProcesso = NULL;
		if(comando%2)
			 pProcesso = criaProcesso(0,0,comando,0,comando,comando,"ls");
		else
			 pProcesso = criaProcesso(0,0,comando,0,comando,comando,"man");
		insereProcesso(pEscalonadorPriopridades,pProcesso);
		comando--;
	}



	executaEscalonamento(pEscalonadorPriopridades);
	
	return 0;
}