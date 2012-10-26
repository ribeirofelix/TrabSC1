

enum TipoEscalonamento
{
	Fifo ,
	SJF ,
	Prioridades ,
	RoudRobin 
};


typedef struct _processo * pProcesso ;

typedef struct _escalonador * pEscalonador ;


pProcesso criaProcesso( int PID , int tempoUCP , int nivelPrioridade , int tempoES , int tempoExecucao,	int tempoEspera , char * nome ) ;

pEscalonador criaEscalonador(enum TipoEscalonamento tpEscalonamento , int timeSlice ) ;

pEscalonador insereProcesso(pEscalonador pEscalonador ,pProcesso pProcesso );

void executaEscalonamento(pEscalonador pEscalonador) ;