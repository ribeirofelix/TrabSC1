#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpretador.h"

typedef struct _processo{
	int PID;
	int tempoUCP;
	int nivelPrioridade;
	int tempoEs;
	int tempoExecucao;
	int tempoEspera;
} Processo;

typedef struct _leitor{
	int qtdComandos;
	char** vetComandos;
	int comandoAtual;
	pProcesso vetProcesso ;
} Leitor;


pLeitor criarLeitor()
{
	pLeitor leitorRet = (pLeitor) malloc(sizeof(Leitor));
	if(leitorRet != NULL)
		return leitorRet;
	
	return NULL;
}

void inicializaLeitor(pLeitor pLeitor)
{
	pLeitor->qtdComandos = 0;
	pLeitor->vetComandos = NULL;
	pLeitor->comandoAtual = 0 ;
	return;
}

int getComandoAtual(pLeitor pleitor)
{
	return pleitor->comandoAtual;
}

char * getComandoN (pLeitor pleitor, int n)
{
	return pleitor->vetComandos[n];
}

//assumo que o leitor está criado e inicializado e o arquivo não é null e que o file é igual ao nome do arquivo
int preencheCommandos (FILE * file, pLeitor pleitor)
{
	int numeroLinhas;
	char * linhaDeComando;
	
	numeroLinhas = contaNumeroLinhasDoArquivo(file);
	pleitor->vetComandos = (char **) malloc ( numeroLinhas*sizeof(char*));
	if (pleitor->vetComandos == NULL)
		return 0;

	pleitor->vetProcesso = (Processo*) malloc (sizeof (Processo*));
	if (pleitor->vetProcesso == NULL)
		return 0;

	/*

	if ( strcmp (nomeDoArquivo, "SJF_comandos.txt"){
		
	}
	if ( strcmp (nomeDoArquivo, "Prioridades_comandos.txt "){
		
	}
	if ( strcmp (nomeDoArquivo, "FIFO_comandos.txt"){
		
	}
	if ( strcmp (nomeDoArquivo, "RoudRobin_comandos.txt"){
		
	}



	*/
	while( fscanf(file,"%[^\n]",linhaDeComando) != EOF)
	{
		pleitor->vetComandos[pleitor->qtdComandos] = (char *) malloc (strlen(linhaDeComando)*sizeof(char));
		if (pleitor->vetComandos[pleitor->qtdComandos] == NULL)
			return 0;

	// TODO preencher o processo

		strcpy(pleitor->vetComandos[pleitor->qtdComandos],linhaDeComando);
		pleitor->qtdComandos++;
	}

	return 1;
}

char* interarComando (pLeitor pleitor)
{
	pleitor->comandoAtual++;

	if (pleitor->comandoAtual > pleitor->qtdComandos)
		return NULL;

	return pleitor->vetComandos[pleitor->comandoAtual - 1];
}

int getQtdComando(pLeitor pleitor)
{
	return pleitor->qtdComandos;
}

static int contaNumeroLinhasDoArquivo (FILE* file)
{
	int numeroLinhas = 0;
	while ( fscanf(file,"%s") != EOF)
		numeroLinhas++;

	return numeroLinhas;
}
