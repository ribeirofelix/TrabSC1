#include <stdio.h>
#include <stdlib.h>

#include "interpretador.h"

int main(){
	
	pProcesso pprocesso;
	pLeitor leitor;
	FILE * arquivo;

	puts("Cria leitor e inicializa");
	leitor = criarLeitor();
	inicializaLeitor(leitor);
	printf("%d %d\n",getComandoAtual(leitor),getQtdComando(leitor) );
	if (getProcessos(leitor) == NULL)
		puts("NULL\n");

	puts("Cria processo e testa");
	pprocesso = criarProcesso();
	inicializaProcesso(pprocesso);
	printf("%d\n",getPID(pprocesso));
	
	/* implementar SJF*/
	/* prioridade */
	/* fifo */
	/* roundbatman */
	return 0;
}