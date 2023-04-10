#ifndef _LIB_TRANSACOES_H
#define _LIB_TRANSACOES_H
#define LINESIZE 1024



struct linha {
    int timestamp;
    int transacao;
    char op;
    char atributo;
}; typedef struct linha linhaT;

struct tabela {
    linhaT* linhaTabela;
}; typedef struct tabela tabelaT;


int comparaCrescente(const void * a, const void *b);

void pegaInfo(FILE *arq ,char line[], int* tamTabela, int* totalTransacoes, int* qntAtributos, char vetorAtributos[]);

void colocaLinhas(tabelaT* tabela,int tamTabela);

void completaTabela(FILE* arq, char line[],tabelaT* tabela);

void liberaTudo(tabelaT* tabela);

void testaAlgoritmos(tabelaT* tabela, int totalTransacoes, int qntAtributos,char vetorAtributos[], int tamTabela);


#endif