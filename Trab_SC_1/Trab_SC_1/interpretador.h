typedef struct _leitor *  pLeitor ;
typedef struct _processo * pProcesso ;

pLeitor criarLeitor();

void inicializaLeitor(pLeitor pLeitor);

pProcesso criarProcesso();

void inicializaProcesso(pProcesso processo);

int getPID(pProcesso processo);

int getComandoAtual(pLeitor pleitor);

int getQtdComando(pLeitor pleitor);

void preencheCommandos (FILE * file, pLeitor pLeitor);

char* interarComando (pLeitor pleitor);