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
	pProcessoInterpretador pProcessoAInserir  ;
	int comando = 5 ;
	
	FILE * entradaRoudRobin  = NULL , * entradaSJF = NULL , * entradaFifo = NULL , *entradaPrioridades = NULL ,*saida = NULL;
	
	printf("apos declarar");
	entradaRoudRobin = fopen("RoudRobin_comandos.txt","rt");
        entradaSJF  = fopen("SJF_comandos.txt","rt");
	entradaFifo = fopen("FIFO_comandos.txt","rt");
	entradaPrioridades  = fopen("Prioridades_comandos.txt","rt");
	saida  = fopen("saida.txt","rt" );
	
	
	if(entradaFifo == NULL || entradaSJF == NULL || entradaPrioridades == NULL || entradaRoudRobin == NULL || saida == NULL )
	{
		printf("erro ao abrir algum arquivo. programa sera finalizado.");
		exit(1);
	}
	printf("antes de preencher");
	preencheCommandos(entradaRoudRobin,leitorFifo,"RoudRobin_comandos.txt");
	preencheCommandos(entradaSJF,leitorSJF,"SJF_comandos.txt");
	preencheCommandos(entradaPrioridades,leitorPrioridades,"Prioridades_comandos.txt");
	preencheCommandos(entradaRoudRobin,leitorRoudRobin,"RoudRobin_comandos.txt");
	
	printf("beforefifo");
	// Itera  sobre cada um dos leitores e depois executa os comandos.

	//FIFO
	
	while ((pProcessoAInserir = iterarComando(leitorFifo)) != NULL)
	{
		pProcesso processoEscalonar = criaProcesso(pProcessoAInserir->PID,pProcessoAInserir->tempoUCP,
			pProcessoAInserir->nivelPrioridade,pProcessoAInserir->tempoEs , pProcessoAInserir->tempoExecucao,pProcessoAInserir->tempoEspera,pProcessoAInserir->nome);
		insereProcesso(pEscalonadorFifo,processoEscalonar);
		free(pProcessoAInserir);
	}
	executaEscalonamento(pEscalonadorFifo);

	//SJF
	while ((pProcessoAInserir = iterarComando(leitorSJF)) != NULL)
	{
		pProcesso processoEscalonar = criaProcesso(pProcessoAInserir->PID,pProcessoAInserir->tempoUCP,
			pProcessoAInserir->nivelPrioridade,pProcessoAInserir->tempoEs , pProcessoAInserir->tempoExecucao,pProcessoAInserir->tempoEspera,pProcessoAInserir->nome);
		insereProcesso(pEscalonadorSJF,processoEscalonar);
		free(pProcessoAInserir);
	}
	executaEscalonamento(pEscalonadorSJF);

	//Prioridades
	while ((pProcessoAInserir = iterarComando(leitorPrioridades)) != NULL)
	{
		pProcesso processoEscalonar = criaProcesso(pProcessoAInserir->PID,pProcessoAInserir->tempoUCP,
			pProcessoAInserir->nivelPrioridade,pProcessoAInserir->tempoEs , pProcessoAInserir->tempoExecucao,pProcessoAInserir->tempoEspera,pProcessoAInserir->nome);
		insereProcesso(pEscalonadorPriopridades,processoEscalonar);
		free(pProcessoAInserir);
	}
	executaEscalonamento(pEscalonadorPriopridades);

	//Round Robin
	while ((pProcessoAInserir = iterarComando(leitorRoudRobin)) != NULL)
	{
		pProcesso processoEscalonar = criaProcesso(pProcessoAInserir->PID,pProcessoAInserir->tempoUCP,
			pProcessoAInserir->nivelPrioridade,pProcessoAInserir->tempoEs , pProcessoAInserir->tempoExecucao,pProcessoAInserir->tempoEspera,pProcessoAInserir->nome);
		insereProcesso(pEscalonadorRoundRobin,processoEscalonar);
		free(pProcessoAInserir);
	}
	executaEscalonamento(pEscalonadorRoundRobin);

	fclose(entradaRoudRobin);
	fclose(pEscalonadorFifo);
	fclose(pEscalonadorSJF);
	fclose(pEscalonadorPriopridades);
	fclose(saida);
	
	
	
	free(pEscalonadorFifo);
	free(pEscalonadorSJF);
	free(pEscalonadorPriopridades);
	free(pEscalonadorRoundRobin);

	
	return 0;
}
