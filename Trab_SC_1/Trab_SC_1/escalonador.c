#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "escalonador.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/stat.h>

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


/*Declara��o das estruturas exportadas pela interface do modulo */
typedef struct _escalonador
{
	enum TipoEscalonamento tipoEscalonamento ;
	int timeSlice ;
	pListaProcessos pLsProcessos ;

}Escalonador;

typedef struct _processo
{
	char * nome ;
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
static void OrdenaListaTempoDecrescente(pListaProcessos pLista);
static void OrdenaListaMaiorPrioridade(pListaProcessos pLista);
static void circularizaLista(pListaProcessos pLista);




pProcesso criaProcesso( int PID , int tempoUCP , int nivelPrioridade , int tempoES , int tempoExecucao,	int tempoEspera , char * nome )
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
	pNovoProcesso->nome = (char *)  malloc(sizeof( strlen(nome) ) ) ;
	strcpy(pNovoProcesso->nome,nome);

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
		executaRoudRobin(pEscalonador);
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
	int segmento;
	segmento= shmget(IPC_PRIVATE,sizeof(struct _lsProcessos), IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR);	 
	pEscalonador->pLsProcessos =(struct _lsProcessos*) shmat(segmento,0,0);
	
	//pEscalonador->pLsProcessos = (struct _lsProcessos *) malloc(sizeof(struct _lsProcessos)) ;
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

	//Se lista vazia
	if(pListaProcessos->qtdProcesso == 0)
	{
		pElemNovo->pProcesso = pProcesso;
		pElemNovo->pAnt = pListaProcessos->pElemInicio;   
		pElemNovo->pProx = pListaProcessos->pElemFim;   
		pListaProcessos->pElemInicio = pElemNovo;   
		pListaProcessos->pElemFim = pElemNovo;   
		pListaProcessos->qtdProcesso++; 
		return ;
	}

	pElemNovo->pProcesso = pProcesso;
	pElemNovo->pAnt = NULL;   
	pElemNovo->pProx = pListaProcessos->pElemInicio;   
	pListaProcessos->pElemInicio->pAnt = pElemNovo;   
	pListaProcessos->pElemInicio = pElemNovo;   
	pListaProcessos->qtdProcesso ++;   




}

pProcesso retiraFimLista(pListaProcessos pListaProcessos  )
{
	pProcesso processoRetirado = pListaProcessos->pElemFim->pProcesso ;
	ElemListaProcessos * ultimoElemento = pListaProcessos->pElemFim ;
	pListaProcessos->pElemFim = pListaProcessos->pElemFim->pAnt ;
	if(!pListaProcessos->qtdProcesso == 1)
		pListaProcessos->pElemFim->pProx = NULL ;
	ultimoElemento->pProcesso = NULL ;
	ultimoElemento->pAnt = NULL;
	free(ultimoElemento);
	pListaProcessos->qtdProcesso --;
	return processoRetirado ;
}


void executaFifo(pEscalonador pEscalonador)
{
	int stats ;
	int qtdProcessos = pEscalonador->pLsProcessos->qtdProcesso ;
	while (pEscalonador->pLsProcessos->qtdProcesso!= 0)
	{
		
		int pId ;
		printf("before fork");
		if((pId = fork() )!= 0)
		{
			waitpid(pId, &stats , 0);
			
		
		}
		else
		{
			pProcesso processo = retiraFimLista(pEscalonador->pLsProcessos);
			char *cmd[] = { processo->nome , (char *)0 };
			char *env[] = { "HOME=/usr/home", "LOGNAME=home", (char *)0 };
			char *path = (char*) malloc(sizeof(strlen("/bin/") + strlen(processo->nome) ) ) ;
			
			strcpy(path,"/bin/");
			strcat(path,processo->nome);
			
			execve (path, cmd, env);

			
		}

	}
}

void executaSJF(pEscalonador pEscalonador)
{
	int stats ;
	int qtdProcessos = pEscalonador->pLsProcessos->qtdProcesso ;
	OrdenaListaTempoDecrescente(pEscalonador->pLsProcessos);
	while (pEscalonador->pLsProcessos->qtdProcesso!= 0)
	{
		
		int pId ;
		
		
		if((pId = fork() )!= 0)
		{
			waitpid(pId, &stats , 0);
					
		}
		else
		{
			pProcesso processo = retiraFimLista(pEscalonador->pLsProcessos);
			char *cmd[] = { processo->nome , (char *)0 };
			char *env[] = { "HOME=/usr/home", "LOGNAME=home", (char *)0 };
			char *path = (char*) malloc(sizeof(strlen("/bin/") + strlen(processo->nome) ) ) ;
			
			strcpy(path,"/bin/");
			strcat(path,processo->nome);
			
			
			execve (path, cmd, env);

		}

	}
}
void executaPrioridade(pEscalonador pEscalonador)
{
	int stats ;
	int qtdProcessos = pEscalonador->pLsProcessos->qtdProcesso ;
	OrdenaListaMaiorPrioridade(pEscalonador->pLsProcessos);
	while (pEscalonador->pLsProcessos->qtdProcesso!= 0)
	{
		
		pid_t pId ;
		
		
		if((pId = fork() )!= 0)
		{
			waitpid(pId, &stats , 0);
			
			
		}
		else
		{
			pProcesso processo = retiraFimLista(pEscalonador->pLsProcessos);
			char *cmd[] = { processo->nome , (char *)0 };
			char *env[] = { "HOME=/usr/home", "LOGNAME=home", (char *)0 };
			char *path = (char*) malloc(sizeof(strlen("/bin/") + strlen(processo->nome) ) ) ;
			
			strcpy(path,"/bin/");
			strcat(path,processo->nome);
			
			execve (path, cmd, env);

			qtdProcessos = pEscalonador->pLsProcessos->qtdProcesso;
		}

	}
}
void executaRoudRobin(pEscalonador pEscalonador)
{
	int stats ;
	printf("oi");
	//OrdenaListaMaiorPrioridade(pEscalonador->pLsProcessos);
	//while (pEscalonador->pLsProcessos->qtdProcesso!= 0)
	//{
	//	
	//	/*int pid = fork();
	//	printf("antes fork");
	//	if(pid)
	//	{
	//		while(TRUE)
	//		{
	//			printf("antes sleep");
	//			sleep(pEscalonador->timeSlice);

	//			kill(pid,SIGSTOP); 

	//			sleep(pEscalonador->timeSlice);

	//			kill(pid,SIGCONT); 
	//		}
	//	}
	//	else
	//	{
	//		int ref = time(NULL);

	//		while(TRUE)
	//		{
	//			pProcesso pProcesso = retiraFimLista(pEscalonador->pLsProcessos);

	//			int tm = time(NULL);

	//			printf("%d PID:%d  Num:%d ", tm-ref,getpid() , pProcesso->nivelPrioridade);
	//			sleep(1);
	//		}
	//	}*/

	//}
	return ;
}

void OrdenaListaTempoDecrescente(pListaProcessos pLista){
	ElemListaProcessos * aux , * aux2;
	pProcesso k;
	for(aux=pLista->pElemInicio ; aux!=NULL ; aux =aux->pProx ){
		
		for(aux2=aux->pProx; aux2!=NULL ; aux2 = aux2->pProx ){
			
			if((aux->pProcesso->tempoExecucao)<(aux2->pProcesso->tempoExecucao)){
				k=aux->pProcesso;
				aux->pProcesso=aux2->pProcesso;
				aux2->pProcesso=k;
			}

		}
	}

}
void OrdenaListaMaiorPrioridade(pListaProcessos pLista){
	ElemListaProcessos * aux , * aux2;
	pProcesso k;
	for(aux=pLista->pElemInicio ; aux!=NULL ; aux =aux->pProx ){
		
		for(aux2=aux->pProx; aux2!=NULL ; aux2 = aux2->pProx ){
			
			if((aux->pProcesso->nivelPrioridade)<(aux2->pProcesso->nivelPrioridade)){
				k=aux->pProcesso;
				aux->pProcesso=aux2->pProcesso;
				aux2->pProcesso=k;
			}

		}
	}

}

void circularizaLista(pListaProcessos pLista)
{
	pLista->pElemFim->pProx = pLista->pElemInicio;
	pLista->pElemInicio->pAnt = pLista->pElemFim;
}
