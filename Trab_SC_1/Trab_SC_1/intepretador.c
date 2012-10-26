#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpretador.h"

#include "escalonador.h"

/* Prototipos de funções encapsuladas no modulo*/
static char * pegaNomeDoProcesso (char* string);
static void adicionaProcessoNaLista (pLeitor pleitor, pProcesso pprocesso);
static void retiraProcessoDaLista (pLeitor pleitor, pProcesso processo);


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
	if(leitorRet != NULL){
		inicializaLeitor(leitorRet);
		return leitorRet;
	}
		
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

void inicializaLeitor(pLeitor  pleitor)
{
	(pleitor)->qtdComandos = 0;
	(pleitor)->vetComandos = NULL;
	(pleitor)->comandoAtual = 0 ;
	(pleitor)->lisProcesso = NULL;
	(pleitor)->proximoProcesso = NULL;
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
	pProcesso pprocesso;
	int numeroLinhas = 0;
	int linhaAtual = 0;
	char * nomeDoProcesso;
	char linhaDeComando[50];

	// conta o numero de linhas.
	while ( fscanf(file,"\n%[^\n]") != EOF)
		numeroLinhas++;

	fclose(file);

	pleitor->vetComandos = (char **) malloc ( numeroLinhas*sizeof(char*));
	if (pleitor->vetComandos == NULL)
	{
		puts("Erro na alocação de memoria");
		return;
	}

	file = fopen (nomeDoArquivo, "rt");
	while ( fscanf(file,"\n%[^\n]", linhaDeComando) != EOF)
	{
		
		pprocesso = criarProcesso();
		inicializaProcesso(pprocesso);

		pleitor->vetComandos[pleitor->qtdComandos] = (char *) malloc (50*sizeof(char));
		if (pleitor->vetComandos[pleitor->qtdComandos] == NULL)
		{
			puts("Erro na alocação de memoria");
			return;
		}

		// caso seja SJF
		if ( strcmp (nomeDoArquivo, "SJF_comandos.txt") == 0){
			int idxIgual;
			char * pIgual;
			char strAux[10];

			// preenche a estrutura do processo
			pIgual = strchr (linhaDeComando, '=');
			idxIgual = pIgual-linhaDeComando+1;
			//tamanhoMili = strlen(linhaDeComando)-idxIgual;

			// strAux aponta para o começo da string de milisegundos
			strcpy (strAux, &linhaDeComando[idxIgual]);

			pprocesso->tempoUCP = strtol( strAux, NULL, 10);
			nomeDoProcesso = pegaNomeDoProcesso(linhaDeComando);
			pprocesso->nome = (char*) malloc (strlen(nomeDoProcesso)*sizeof(char));
			if(pprocesso->nome == NULL)
			{
				puts("Erro na alocação de memoria");
				return;
			}

			strcpy(pprocesso->nome, nomeDoProcesso);
			pprocesso->PID = linhaAtual;

		}
		else // Caso seja prioridade
			if ( strcmp (nomeDoArquivo, "Prioridades_comandos.txt") == 0)
			{
				char * pIgual;
				int idxIgual;

				// preenche a estrutura do processo
				pIgual = strchr (linhaDeComando, '=');
				idxIgual = pIgual-linhaDeComando+1;
				pprocesso->nivelPrioridade = atoi(&linhaDeComando[idxIgual]);


				nomeDoProcesso = pegaNomeDoProcesso(linhaDeComando);
				pprocesso->nome = (char*) malloc (strlen(nomeDoProcesso)*sizeof(char));
				if(pprocesso->nome == NULL)
				{
					puts("Erro na alocação de memoria");
					return;
				}

				strcpy(pprocesso->nome, nomeDoProcesso);
				pprocesso->PID = linhaAtual;

			}
			else // caso seja fifo ou roudrobin
				if ( strcmp (nomeDoArquivo, "FIFO_comandos.txt") == 0 || 
					strcmp (nomeDoArquivo, "RoudRobin_comandos.txt") == 0)
				{
					nomeDoProcesso = &linhaDeComando[5];

					pprocesso->nome = (char*) malloc (strlen(nomeDoProcesso)*sizeof(char));
					if(pprocesso->nome == NULL)
					{
						puts("Erro na alocação de memoria");
						return;
					}

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

pProcesso iterarComando (pLeitor pleitor)
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

 void retiraProcessoDaLista (pLeitor pleitor, pProcesso processo)
{
	pProcesso aux, anterior = NULL;

	/* Leva aux para o processo a ser excluido o anterior para o processo anterior a aux */
	for (aux = pleitor->lisProcesso; aux != processo; anterior = aux, aux = aux->proximo);

	if (anterior != NULL)
	{
		anterior->proximo = aux->proximo;
		aux->proximo = NULL;
		return;
	}

	pleitor->lisProcesso = aux->proximo;
	aux->proximo = NULL;

}

 char * pegaNomeDoProcesso (char* string)
{

	char * resultado, * nome, * segundoEspaco;
	resultado = &string[5];

	segundoEspaco = strchr (resultado, ' ');
	if (segundoEspaco == NULL)
	{
		nome = (char *) malloc (strlen(resultado)*sizeof(char));

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

 void adicionaProcessoNaLista (pLeitor pleitor, pProcesso pprocesso)
{
	pProcesso aux = pleitor->lisProcesso;

	if (aux == NULL)
	{
		pleitor->lisProcesso = pprocesso;
	}
	else
	{
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
}