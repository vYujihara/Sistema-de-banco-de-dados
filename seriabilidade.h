#ifndef _SERIABILIDADE_H
#define _SERIABILIDADE_H


#define M 100

int ehCicloAux(int matriz[M][M] ,int v , int nodosVisitados[], int nodosProcessados[], int totalCommits);

int ehCiclo(int matriz[M][M], int totalCommits);

void pegaIndice(int commits[],int totalCommits, int ti, int tj,int* linha, int* coluna);

int matrizVazia(int matriz[M][M], int totalCommits);

int verificaConflito(tabelaT* tabela, int tamTabela, int commits[],int totalCommits,int matriz[M][M],int qntAtributos,char vetorAtributos[]);

int ocorreuRegra(tabelaT* tabela ,char op, char atributo, int* tj,int* ti, int k,int commits[],int totalCommits);

void testeSeriabilidade(tabelaT* tabela, int totalTransacoes, int qntAtributos,char vetorAtributos[],int tamTabela);
#endif