#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpretador.h"

typedef struct _processo{
	char* nome;
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
	pProcesso* vetProcesso ;
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
	pLeitor->vetProcesso = NULL;
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

pProcesso inicializaProcesso (pProcesso processo)
{
	processo->nome;
	processo->nivelPrioridade = 0;
	processo->PID = 0;
	processo->tempoUCP = 0;
	processo->tempoEs = 0;
	processo->tempoExecucao = 0;
	processo->tempoEspera = 0;
}

//assumo que o leitor está criado e inicializado e o arquivo não é null e que o file é igual ao nome do arquivo
int preencheCommandos (FILE * file, pLeitor pleitor, char * nomeDoArquivo)
{
	int numeroLinhas;
	int linhaAtual = 0;
	char * linhaDeComando;

	numeroLinhas = contaNumeroLinhasDoArquivo(file);
	pleitor->vetComandos = (char **) malloc ( numeroLinhas*sizeof(char*));
	if (pleitor->vetComandos == NULL)
		return 0;

	pleitor->vetProcesso = (Processo**) malloc (numeroLinhas*sizeof (Processo*));
	if (pleitor->vetProcesso == NULL)
		return 0;

	while( fscanf(file,"%[^\n]",linhaDeComando) != EOF)
	{
		pProcesso pprocesso;
		pprocesso = (pProcesso) malloc (sizeof(pProcesso));
		if (pprocesso == NULL)
			return 0;

		pleitor->vetComandos[pleitor->qtdComandos] = (char *) malloc (strlen(linhaDeComando)*sizeof(char));
		if (pleitor->vetComandos[pleitor->qtdComandos] == NULL)
			return 0;
		
		if ( strcmp (nomeDoArquivo, "SJF_comandos.txt")){
			
		}
		else if ( strcmp (nomeDoArquivo, "Prioridades_comandos.txt ")){
			char strAux[2];
			
			// preencher a estrutura do processo
			strAux[0] = linhaDeComando[strlen(linhaDeComando)-1]; strAux[1] = '\0';
			pprocesso->nivelPrioridade = strtol( strAux, NULL, 10);
			strcpy(pprocesso->nome, pegaNomeDoProcesso(linhaDeComando));
			pprocesso->PID = linhaAtual;
 		}
		else if ( strcmp (nomeDoArquivo, "FIFO_comandos.txt")){

		}
		else if ( strcmp (nomeDoArquivo, "RoudRobin_comandos.txt")){

		}

		// salva o processo no vetord
		pleitor->vetProcesso[linhaAtual] = pprocesso;
		
		strcpy(pleitor->vetComandos[pleitor->qtdComandos],linhaDeComando);
		pleitor->qtdComandos++;
	
		linhaAtual++;
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

static char * pegaNomeDoProcesso (char* string)
{
	char* resultado;
	int i, j, segundoEspaco;

	for (i = 5; ; i++)
	{
		if (string[i] == ' ')
		{
			segundoEspaco = i; 
			break;
		}
	}

	resultado = (char*) malloc ((segundoEspaco-3)*sizeof(char));
	if (resultado == NULL)
		return NULL;

	for (i = 5, j = 0; i < segundoEspaco; i++, j++)
	{
		resultado[j] = string[i];
	}
	resultado[j+1] = '\0';

	return resultado;
}