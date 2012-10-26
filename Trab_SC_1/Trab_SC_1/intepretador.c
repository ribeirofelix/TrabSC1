#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpretador.h"

#include "escalonador.h"


/* Prototipos de funções encapsuladas no modulo*/
static int contaNumeroLinhasDoArquivo (FILE* file);
static char * pegaNomeDoProcesso (char* string);
static void adicionaProcessoNaLista (pLeitor pleitor, pProcesso pprocesso);
static void retiraProcessoDaLista (pLeitor pleitor, pProcesso processo);


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

typedef struct _leitor{
	int qtdComandos;
	char** vetComandos;
	int comandoAtual;
	pProcesso lisProcesso;
	pProcesso proximoProcesso;
} Leitor;


pProcesso criarProcesso ()
{
	pProcesso processoRet = (pProcesso) malloc(sizeof(Processo));
	if(processoRet != NULL)
		return processoRet;

	return NULL;
}

pLeitor criarLeitor()
{
	pLeitor leitorRet = (pLeitor) malloc(sizeof(Leitor));
	if(leitorRet != NULL)
		return leitorRet;

	return NULL;
}

void inicializaProcesso (pProcesso processo)
{
	processo->nome;
	processo->nivelPrioridade = 0;
	processo->PID = 0;
	processo->tempoUCP = 0;
	processo->tempoEs = 0;
	processo->tempoExecucao = 0;
	processo->tempoEspera = 0;
	processo->proximo = NULL;
}

void inicializaLeitor(pLeitor pLeitor)
{
	pLeitor->qtdComandos = 0;
	pLeitor->vetComandos = NULL;
	pLeitor->comandoAtual = 0 ;
	pLeitor->lisProcesso = NULL;
	pLeitor->proximoProcesso = NULL;
	return;
}

int getPID(pProcesso processo)
{
	return processo->PID;
}

int getComandoAtual(pLeitor pleitor)
{
	return pleitor->comandoAtual;
}

char * getComandoN (pLeitor pleitor, int n)
{
	return pleitor->vetComandos[n];
}

pProcesso getProcessos (pLeitor pleitor)
{
	return pleitor->lisProcesso;
}

//assumo que o leitor está criado e inicializado e o arquivo não é null e que o file é igual ao nome do arquivo
void preencheCommandos (FILE * file, pLeitor pleitor, char * nomeDoArquivo)
{
	int numeroLinhas;
	int linhaAtual = 0;
	char * linhaDeComando;

	numeroLinhas = contaNumeroLinhasDoArquivo(file);
	pleitor->vetComandos = (char **) malloc ( numeroLinhas*sizeof(char*));
	if (pleitor->vetComandos == NULL)
	{
		puts("Erro na alocação de memoria");
		return;
	}

	while( fscanf(file,"%[^\n]",linhaDeComando) != EOF)
	{
		pProcesso pprocesso;
		pprocesso = criarProcesso();

		pleitor->vetComandos[pleitor->qtdComandos] = (char *) malloc (strlen(linhaDeComando)*sizeof(char));
		if (pleitor->vetComandos[pleitor->qtdComandos] == NULL)
		{
			puts("Erro na alocação de memoria");
			return;
		}

		// caso seja SJF
		if ( strcmp (nomeDoArquivo, "SJF_comandos.txt")){
			int idxIgual, tamanhoMili;
			char * pIgual;
			char * strAux;

			// preenche a estrutura do processo
			pIgual = strchr (linhaDeComando, '=');
			idxIgual = pIgual-linhaDeComando+1;
			tamanhoMili = strlen(linhaDeComando)-idxIgual;

			// strAux aponta para o começo da string de milisegundos
			strAux = &linhaDeComando[idxIgual+1];

			pprocesso->tempoUCP = strtol( strAux, NULL, 10);
			strcpy(pprocesso->nome, pegaNomeDoProcesso(linhaDeComando));
			pprocesso->PID = linhaAtual;

		}
		else // Caso seja prioridade
			if ( strcmp (nomeDoArquivo, "Prioridades_comandos.txt ") == 0)
			{
				char * pIgual;
				int idxIgual;

				// preenche a estrutura do processo
				pIgual = strchr (linhaDeComando, '=');
				idxIgual = pIgual-linhaDeComando+1;
				pprocesso->nivelPrioridade = atoi(&linhaDeComando[idxIgual+1]);
				strcpy(pprocesso->nome, pegaNomeDoProcesso(linhaDeComando));
				pprocesso->PID = linhaAtual;

			}
			else // caso seja fifo ou roudrobin
				if ( strcmp (nomeDoArquivo, "FIFO_comandos.txt") == 0 || 
					strcmp (nomeDoArquivo, "RoudRobin_comandos.txt") == 0)
				{
					char * nomeDoProcesso = &linhaDeComando[5];

					strcpy(pprocesso->nome, nomeDoProcesso);
					pprocesso->PID = linhaAtual;
				}

				// salva o processo no vetor do leitor
				adicionaProcessoNaLista(pleitor, pprocesso);

				strcpy(pleitor->vetComandos[pleitor->qtdComandos],linhaDeComando);
				pleitor->qtdComandos++;

				linhaAtual++;
	}
}

pProcesso interarComando (pLeitor pleitor)
{
	int i;
	pProcesso aux;

	if (pleitor->comandoAtual > pleitor->qtdComandos)
		return NULL;

	for (aux = pleitor->lisProcesso, i = 0; i < pleitor->comandoAtual-1;  aux = aux->proximo, i++);

	retiraProcessoDaLista (pleitor, aux);

	pleitor->comandoAtual++;

	return aux;
}

int getQtdComando(pLeitor pleitor)
{
	return pleitor->qtdComandos;
}

static void retiraProcessoDaLista (pLeitor pleitor, pProcesso processo)
{
	pProcesso aux, anterior;
	
	/* Leva aux para o processo a ser excluido o anterior para o processo anterior a aux */
	for (aux = pleitor->lisProcesso; aux != processo; anterior = aux, aux = aux->proximo);

	anterior->proximo = aux->proximo;
	aux->proximo = NULL;

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

	char * resultado, * nome, * segundoEspaco;
	resultado = &string[5];
	
	segundoEspaco = strchr (resultado, ' ');
	if (segundoEspaco == NULL)
	{
		strcpy (nome, resultado);
		return nome;
	
	}else 
	{
		int idxSegundoEspaco;
		int i;
		idxSegundoEspaco = segundoEspaco-resultado+1;
		
		nome = (char*) malloc ((idxSegundoEspaco+1)*sizeof(char));
		if (nome == NULL)
			return NULL;


		for (i = 0; i < idxSegundoEspaco; i++)
		{
			nome[i] = resultado [i];
		}
		nome[i] = '\0';

		return nome;
	}
}

static void adicionaProcessoNaLista (pLeitor pleitor, pProcesso pprocesso)
{
	pProcesso aux = pleitor->lisProcesso;
	while (aux != NULL)
	{
		if (aux->proximo == NULL)
		{
			aux->proximo = pprocesso;
			break;
		}
		aux = aux->proximo;
	}
}