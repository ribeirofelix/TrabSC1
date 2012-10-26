#include <stdio.h>
typedef struct _leitor *  pLeitor ;
typedef struct _processoInterpretador * pProcessoInterpretador ;

typedef struct _processoInterpretador{
	char* nome;
	int PID;
	int tempoUCP;
	int nivelPrioridade;
	int tempoEs;
	int tempoExecucao;
	int tempoEspera;
	pProcessoInterpretador proximo;
} ProcessoInterpretador;

pLeitor criarLeitor();

void inicializaLeitor(pLeitor pLeitor);

pProcessoInterpretador getProcessos (pLeitor pleitor);

void inicializaProcesso(pProcessoInterpretador processo);

int getPID(pProcessoInterpretador processo);

int getComandoAtual(pLeitor pleitor);

int getQtdComando(pLeitor pleitor);

void preencheCommandos (FILE * file, pLeitor pLeitor, char* nomeArquivo);

pProcessoInterpretador iterarComando (pLeitor pleitor);