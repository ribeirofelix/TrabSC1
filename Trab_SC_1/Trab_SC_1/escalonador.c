#include <stdio.h>
#include <stdlib.h>
#include "escalonador.h"
#include <sys/types.h>

#define TRUE 1
#define FALSE 0


#define fork() printf("fork;")
#define execve() printf("execve")


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


/*Declara��o das estruturas exportadas pela interface do modulo */
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
/*Fim da declara��o estruturas exportadas*/


/* Declara��o dos prototipos das fun��es encapsuladas no modulo */
static void inicializaLista(pEscalonador pEscalonador);
static void insereInicioLista(pListaProcessos pListaProcessos , pProcesso pProcesso) ;
static void executaFifo(pEscalonador pEscalonador);
static void executaSJF(pEscalonador pEscalonador);
static void executaPrioridade(pEscalonador pEscalonador);
static void executaRoudRobin(pEscalonador pEscalonador);
static pProcesso retiraFimLista(pListaProcessos pListaProcessos  );
static pProcesso retiraProcessoMenorTempo(pListaProcessos pListaProcessos);
static pProcesso retiraProcessoMaiorPrioridade(pListaProcessos pListaProcessos);

pProcesso criaProcesso( int PID , int tempoUCP , int nivelPrioridade , int tempoES , int tempoExecucao,	int tempoEspera )
{
	pProcesso pNovoProcesso = (pProcesso) malloc(sizeof(Processo));
	if(pNovoProcesso == NULL)
	{
		printf("Erro ao criar processo. Programa ser� finalizado.");
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
		executaSJF(pEscalonador);
		break;
	case Prioridades :
		executaPrioridade(pEscalonador);
		break;
	case Fifo :
		executaFifo(pEscalonador);
		break;
	case RoudRobin:
		executaFifo(pEscalonador);
		break;
	default:
		printf("Politica errada. Programa sera finalizado.");
		exit(1);
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

pProcesso retiraFimLista(pListaProcessos pListaProcessos  )
{
	pProcesso processoRetirado = pListaProcessos->pElemFim->pProcesso ;
	ElemListaProcessos * ultimoElemento = pListaProcessos->pElemFim ;
	pListaProcessos->pElemFim = pListaProcessos->pElemFim->pAnt ;
	pListaProcessos->pElemFim->pProx = NULL ;
	ultimoElemento->pProcesso = NULL ;
	free(ultimoElemento);
	return processoRetirado ;
}


void executaFifo(pEscalonador pEscalonador)
{
	int stats ;
	int qtdElementos = pEscalonador->pLsProcessos->qtdProcesso;
	while (qtdElementos != 0)
	{
		pProcesso processo = retiraFimLista(pEscalonador->pLsProcessos);
		if(fork() != 0)
		{
			//waitpid(-1 , &stats , 0);
		}
		else
		{
			execve();
		}

	}
}

void executaSJF(pEscalonador pEscalonador)
{
	int stats ;
	int qtdProcessos = pEscalonador->pLsProcessos->qtdProcesso ;
	while (qtdProcessos != 0)
	{
		pProcesso processAtual = retiraProcessoMenorTempo(pEscalonador->pLsProcessos);
		if (fork() != 0)
		{
			//waitpid(-1,&stats,0);
		}
		else
		{
			execve();
		}
	}
	return ;
}
void executaPrioridade(pEscalonador pEscalonador)
{
	return ;
}
void executaRoudRobin(pEscalonador pEscalonador)
{
	return ;
}


pProcesso retiraProcessoMaiorPrioridade(pListaProcessos pListaProcessos)
{
	ElemListaProcessos * pElemIteracao = pListaProcessos->pElemInicio;
	int menorTempo = pElemIteracao->pProcesso->nivelPrioridade ;
	pProcesso processoRet = NULL ;
	
	// desfaz ligacao das pontas da lista
	pListaProcessos->pElemInicio->pAnt = NULL ;
	pListaProcessos->pElemFim->pProx = NULL ;

	//Procura Mais prioritario
	while (pElemIteracao != NULL )
	{
		if(pElemIteracao->pProcesso->nivelPrioridade > pElemIteracao->pProx->pProcesso->nivelPrioridade )
		{
			pElemIteracao = pElemIteracao->pProx ;
		}
	}
	//Retira da Lista
	if(pElemIteracao != NULL)
	{
		ElemListaProcessos * pTempAnt = pElemIteracao->pAnt ;
		ElemListaProcessos * pTempProx = pElemIteracao->pProx ;
		

		pTempAnt->pProx = pTempProx ;
		pTempProx->pAnt = pTempAnt ;

		pElemIteracao->pAnt = NULL ;
		pElemIteracao->pProx = NULL ;
		processoRet = pElemIteracao->pProcesso ;
		pElemIteracao->pProcesso = NULL ;
		free(pElemIteracao);
		pListaProcessos->qtdProcesso --;
	}

	return processoRet ;
}
pProcesso retiraProcessoMenorTempo(pListaProcessos pListaProcessos)
{
	ElemListaProcessos * pElemIteracao = pListaProcessos->pElemInicio;
	int menorTempo = pListaProcessos->pElemInicio->pProcesso->tempoExecucao ;
	pProcesso processoRet = NULL ;
	
	// desfaz ligacao das pontas da lista
	pListaProcessos->pElemInicio->pAnt = NULL ;
	pListaProcessos->pElemFim->pProx = NULL ;

	//Procura Menor Tempo
	while (pElemIteracao != NULL )
	{
		if(pElemIteracao->pProcesso->tempoExecucao > pElemIteracao->pProx->pProcesso->tempoExecucao )
		{
			pElemIteracao = pElemIteracao->pProx ;
		}
	}
	//Retira da Lista
	if(pElemIteracao != NULL)
	{
		ElemListaProcessos * pTempAnt = pElemIteracao->pAnt ;
		ElemListaProcessos * pTempProx = pElemIteracao->pProx ;
		

		pTempAnt->pProx = pTempProx ;
		pTempProx->pAnt = pTempAnt ;

		pElemIteracao->pAnt = NULL ;
		pElemIteracao->pProx = NULL ;
		processoRet = pElemIteracao->pProcesso ;
		pElemIteracao->pProcesso = NULL ;
		free(pElemIteracao);
		pListaProcessos->qtdProcesso --;
	}

	return processoRet ;
}