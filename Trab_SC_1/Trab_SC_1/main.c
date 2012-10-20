#include <stdio.h>
#include <stdlib.h>

#include "interpretador.h"

int main(){

	pLeitor leitor = criarLeitor();
	inicializaLeitor(leitor);
	printf("%d %d",getComandoAtual(leitor),getQtdComando(leitor) );
	return 0;
}