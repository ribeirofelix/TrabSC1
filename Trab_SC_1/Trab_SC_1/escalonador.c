#include <stdio.h>
#include <stdlib.h>
#include "escalonador.h"

#define TRUE 1
#define FALSE 0

/*Esturturas declaradas e encapsuladas no modulo*/
typedef struct _elemListaProcessos
{
	pProcesso pProcesso ;

	struct _elemListaProcessos * pAnt ;

	struct  _elemListaProcessos * pProx ;

}ElemListaProcessos;


typedef struct _lsProcessos
{
	int qtdProcesso;
	ElemListaProcessos * pElemCorr ;
	ElemListaProcessos * pElemInicio ;
	ElemListaProcessos * pElemFim ;

}ListaProcessos;

typedef struct _lsProcessos * pListaProcessos ;
/*Fim das estruturas encapsuladas no modulo*/


/*Declaração das estruturas exportadas pela interface do modulo */
typedef struct _escalonador
{
	enum TipoEscalonamento tipoEscalonamento ;
	int timeSlice ;
	pListaProcessos pLsProcessos ;
	
}Escalonador;

typedef struct _processo
{
	int PID ;
	int tempoUCP ;
	int nivelPrioridade ;
	int tempoES ;
	int tempoExecucao ;
	int tempoEspera ;
}Processo;
/*Fim da declaração estruturas exportadas*/


/* Declaração dos prototipos das funções encapsuladas no modulo */
static void inicializaLista(pEscalonador pEscalonador);
static void insereInicioLista(pListaProcessos pListaProcessos , pProcesso pProcesso) ;
static void executaFifo(pEscalonador pEscalonador);
static void executaSJF(pEscalonador pEscalonador);
static void executaPrioridade(pEscalonador pEscalonador);
static void executaRoudRobin(pEscalonador pEscalonador);



pProcesso criaProcesso( int PID , int tempoUCP , int nivelPrioridade , int tempoES , int tempoExecucao,	int tempoEspera )
{
	pProcesso pNovoProcesso = (pProcesso) malloc(sizeof(Processo));
	if(pNovoProcesso == NULL)
	{
		printf("Erro ao criar processo. Programa será finalizado.");
		exit(1);
	}

	pNovoProcesso->PID = PID;
	pNovoProcesso->tempoUCP = tempoUCP;
	pNovoProcesso->nivelPrioridade = nivelPrioridade;
	pNovoProcesso->tempoES = tempoES;
	pNovoProcesso->tempoExecucao = tempoExecucao;
	pNovoProcesso->tempoEspera = tempoEspera;

	return pNovoProcesso;


}

pEscalonador criaEscalonador( enum TipoEscalonamento tpEscalonamento , int timeSlice )
{
	pEscalonador pNovoEscalonador = (pEscalonador) malloc(sizeof(Escalonador));
	if(pNovoEscalonador == NULL)
	{
		printf("Erro ao criar Escalonador. Programa sera finalizado.");
		exit(1);
	}

	pNovoEscalonador->tipoEscalonamento = tpEscalonamento;
	pNovoEscalonador->timeSlice = timeSlice;
	inicializaLista(pNovoEscalonador);
	return pNovoEscalonador;
}

pEscalonador insereProcesso(pEscalonador pEscalonador ,pProcesso pProcesso )
{
	if(pProcesso == NULL)
		return NULL ;

	insereInicioLista(pEscalonador->pLsProcessos , pProcesso );
	

}

void executaEscalonamento(pEscalonador pEscalonador)
{
	switch (pEscalonador->tipoEscalonamento)
	{
	case  SJF :
		break;
	case Prioridades :
		break;
	case Fifo :
		break;
	case RoudRobin:
		break;
	default:
		break;
	}
	return;
}

void inicializaLista(pEscalonador pEscalonador)
{
	pEscalonador->pLsProcessos = (pListaProcessos) malloc(sizeof(struct _lsProcessos ) );
	pEscalonador->pLsProcessos->pElemCorr = NULL ;
	pEscalonador->pLsProcessos->pElemFim = NULL ;
	pEscalonador->pLsProcessos->pElemInicio = NULL ;
	pEscalonador->pLsProcessos->qtdProcesso = 0 ;
	
}


void insereInicioLista(pListaProcessos pListaProcessos , pProcesso pProcesso)
{
	ElemListaProcessos * pElemNovo = (ElemListaProcessos *) malloc(sizeof(ElemListaProcessos));
	if(pElemNovo == NULL)
	{
		printf("Erro ao criar novo Elemento da lista de processos. Programa sera finalizado.");
		exit(1);
	}

	pElemNovo->pProcesso = pProcesso;
	pElemNovo->pProx = pListaProcessos->pElemInicio ;
	pElemNovo->pAnt = NULL ;


	pListaProcessos->pElemCorr = pElemNovo ;
	pListaProcessos->pElemInicio = pElemNovo ;
	
	
}




