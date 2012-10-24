#include <stdio.h>
#include <stdlib.h>

#include "interpretador.h"

int main(){

	pLeitor leitor = criarLeitor();
	
	FILE * entrada = fopen("E:\\Documentos\\entrada.txt","rt");
	char texto[100] ;
	inicializaLeitor(leitor);
	while (fscanf(entrada,"exec %s",texto) != EOF  )
	{

		printf("%s",texto);
	}
	printf("%d %d",getComandoAtual(leitor),getQtdComando(leitor) );
	return 0;
}