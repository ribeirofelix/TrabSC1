typedef struct _leitor *  pLeitor ;
typedef struct _processo * pProcesso ;

typedef struct _processo{
	char* nome;
	int PID;
	int tempoUCP;
	int nivelPrioridade;
	int tempoEs;
	int tempoExecucao;
	int tempoEspera;
	pProcesso proximo;
} Processo;

pLeitor criarLeitor();

void inicializaLeitor(pLeitor pLeitor);

pProcesso getProcessos (pLeitor pleitor);

void inicializaProcesso(pProcesso processo);

int getPID(pProcesso processo);

int getComandoAtual(pLeitor pleitor);

int getQtdComando(pLeitor pleitor);

void preencheCommandos (FILE * file, pLeitor pLeitor, char* nomeArquivo);

pProcesso iterarComando (pLeitor pleitor);