#include <stdio.h>
#include <stdlib.h>
#include "interpretador.h"
#include "escalonador.h"
int main(){
	
	pLeitor leitorFifo = criarLeitor();
	pLeitor leitorSJF = criarLeitor();
	pLeitor leitorPrioridades = criarLeitor();
	pLeitor leitorRoudRobin = criarLeitor();
	
	pEscalonador pEscalonadorFifo = criaEscalonador( Fifo,0);
	pEscalonador pEscalonadorSJF = criaEscalonador(SJF,0);
	pEscalonador pEscalonadorPriopridades = criaEscalonador(Prioridades,0);
	pEscalonador pEscalonadorRoundRobin = criaEscalonador(RoudRobin,5);
	pProcesso pProcessoAInserir  ;
	int comando = 5 ;
	//printf("oi1");


	FILE * entradaFifo = fopen("E:\\FIFO_comandos.txt","r");
	if(entradaFifo == NULL)
		exit(1);
	/*FILE * entradaSJF = fopen("SJF_comandos.txt","rt");

	FILE * entradaFifo = fopen("FIFO_comandos.txt","rt");
	FILE * entradaSJF = fopen("SJF_comandos.txt","rt");

	FILE * entradaPrioridades = fopen("Prioridades_comandos.txt","rt");*/
	FILE * entradaRoudRobin = fopen("RoudRobin_comandos.txt","rt");
	/*FILE * saida = fopen("E:\\Documento\\saida.txt","rt" ); */

	preencheCommandos(entradaRoudRobin,leitorFifo,"RoudRobin_comandos.txt");

	/*preencheCommandos(entradaSJF,leitorSJF,"SJF_comandos.txt");
	preencheCommandos(entradaPrioridades,leitorPrioridades,"Prioridades_comandos.txt");
	preencheCommandos(entradaRoudRobin,leitorRoudRobin,"RoudRobin_comandos.txt");
	
*/
	// Itera  sobre cada um dos leitores e depois executa os comandos.

	//FIFO
	

	//SJF

	//Prioridades

	//Round Robin





	/*while ( (pProcessoAInserir = interarComando(leitor) ) != NULL )
	{
		insereProcesso(pEscalonadorFifo,pProcessoAInserir);
	}*/

	/*while (comando != 0)
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
*/
	comando = 5 ;
	while (comando != 0)
	{
		
		pProcesso pProcesso = NULL;
		printf("oi");
		if(comando%2)
			 pProcesso = criaProcesso(0,0,comando,0,comando,comando,"ls");
		else
			 pProcesso = criaProcesso(0,0,comando,0,comando,comando,"man");
		insereProcesso(pEscalonadorRoundRobin,pProcesso);
		comando--;
	}

	executaEscalonamento(pEscalonadorRoundRobin);
	
	return 0;
}