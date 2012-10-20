typedef struct _leitor *  pLeitor ;


pLeitor criarLeitor();

void inicializaLeitor(pLeitor pLeitor);

int getComandoAtual(pLeitor pleitor);
int getQtdComando(pLeitor pleitor);