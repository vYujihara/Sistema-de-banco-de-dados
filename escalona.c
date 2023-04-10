#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libEscalona.h"
#include "equivalente.h"
#include "seriabilidade.h"


int main (int argc, char** argv){
    FILE* arq;
    int tamTabela = 0;
    int totalTransacoes = 1;
    char vetorAtributos[10];
    int qntAtributos = 0;
    char line[LINESIZE+1];
    tabelaT* tabela;

    arq = fopen(argv[1],"r");
    if (!arq){
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    pegaInfo(arq, line, &tamTabela, &totalTransacoes,&qntAtributos, vetorAtributos);
    tabela = malloc(sizeof(tabelaT));
    colocaLinhas(tabela, tamTabela);
    completaTabela(arq,line,tabela);
    testaAlgoritmos(tabela,totalTransacoes,qntAtributos,vetorAtributos,tamTabela);
    fclose(arq);
    liberaTudo(tabela);
    return 0;
}