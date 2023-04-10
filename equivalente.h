#ifndef _EQUIVALENTE_H
#define _EQUIVALENTE_H

struct equivalencia {
    int leitura;
    int ultimaEscrita;
}; typedef struct equivalencia equivalenciaT;

struct regras {
    int ti;
    int tj;
    char atributo;
}; typedef struct regras regrasT;

void completaVisoes(tabelaT* tabela,tabelaT* visoes, int commits[], int totalCommits, int visaoIndice,int tamTabela);

void troca(int* a, int *b);

void permutacao(tabelaT* tabela,tabelaT* visoes, int commits[], int indice, int n, int* visaoIndice, int tamTabela,int tamVisao);

int retornaTamanhoVisao(tabelaT* tabela, int tamTabela,int commits[], int totalCommits);

int fat (int n);

int testaRegra(tabelaT* tabela,char op, char atributo, int* tj, int* ti, int k,int commits[],int totalCommits);

int verificaTj(int commits[], int totalCommits,int tj);

void regraUltimaEscrita(tabelaT* tabela,int lastEscrita[], int commits[], int totalCommits,int tj, int indice);

int ehEquivalente(tabelaT* visoes, int nVisoes,int tamVisao ,equivalenciaT* equivalente, regrasT* vetorRegras, int contVetorRegras,int lastEscrita[], char vetorAtributos[],int qntAtributos);


void freeAll(tabelaT* visoes,int nVisoes, equivalenciaT* equivalente, regrasT* vetorRegras);

int igual(int ini, tabelaT* tabela, tabelaT* visoes, int tamVisao,int nVisoes);

int verificaEquivalencia(tabelaT* tabela, int tamTabela, int commits[],int totalCommits,int qntAtributos,char vetorAtributos[],int ini);

#endif