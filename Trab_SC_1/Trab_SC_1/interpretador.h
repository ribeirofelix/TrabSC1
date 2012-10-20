typedef struct _leitor *  pLeitor ;
typedef struct _processo * pProcesso ;

pLeitor criarLeitor();

void inicializaLeitor(pLeitor pLeitor);

int getComandoAtual(pLeitor pleitor);

int getQtdComando(pLeitor pleitor);

int preencheCommandos (FILE * file, pLeitor pLeitor);

char* interarComando (pLeitor pleitor);