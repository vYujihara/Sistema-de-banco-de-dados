#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libEscalona.h"
#include "equivalente.h"
#include "seriabilidade.h"



int comparaCrescente(const void * a, const void *b){
    if (*(int*) a < *(int*)b)
        return -1;
    else
        return 1;
}

/* Percorre o arquivo e pega numero de transacoes,atributos  e quantas instrucoes */
void pegaInfo(FILE *arq ,char line[], int* tamTabela, int* totalTransacoes, int* qntAtributos,char vetorAtributos[] ){
    int numTransacao = 0;
    char atributo = ' ';
    int i;
    int novoAtributo;

    fgets(line,LINESIZE,arq);
    while(!feof(arq) ){
        novoAtributo = 1;
        sscanf(line,"%*d %d %*c %c",&numTransacao,&atributo);
        for(i = 0 ; i < *qntAtributos ; i++){
            if (vetorAtributos[i] == atributo ||  atributo == '-' ){
                novoAtributo = 0;
            }
        }
        if (novoAtributo){
            vetorAtributos[*qntAtributos]= atributo;
            (*qntAtributos)++;
        }
        
        if (*totalTransacoes == numTransacao){
            (*totalTransacoes)++;
        }
        if ( (!qntAtributos) && (atributo != '-') ){
            vetorAtributos[*qntAtributos] = atributo;
            (*qntAtributos)++;
        }
        (*tamTabela)++;
        fgets(line,LINESIZE,arq);
    }
    rewind(arq);
}

/* Aloca linhas de acordo com tamTabela*/
void colocaLinhas(tabelaT* tabela,int tamTabela){
        tabela->linhaTabela= malloc(sizeof(linhaT) * tamTabela);
}

/* Percorre o txt e guarda as informações */
void completaTabela(FILE* arq, char line[],tabelaT* tabela){
    int cont = 0;
    int time = 0;
    int transac = 0;
    char op;
    char atributo;

    fgets(line,LINESIZE,arq);
    while(!feof(arq) ){
        sscanf(line,"%d %d %c %c",&time, &transac, &op, &atributo);
        tabela->linhaTabela[cont].timestamp = time;
        tabela->linhaTabela[cont].transacao = transac;
        tabela->linhaTabela[cont].op = op;
        tabela->linhaTabela[cont].atributo = atributo;
        fgets(line,LINESIZE,arq);
        cont++;
    }
}


/* função que implementa os 2 algoritmos */
void testaAlgoritmos(tabelaT* tabela, int totalTransacoes, int qntAtributos,char vetorAtributos[], int tamTabela){
    int j,i;
    int commits[20];
    int totalCommits = 0;
    int deuCommit = 0;
    int matriz[M][M];
    char equivalente[3] = " ";
    char serial[3] = " ";
    int saida = 1;
    int ini = 0;

    memset(commits, 0 , sizeof(commits));
    
    /* Se achou uma operacao do tipo C, guarda a transacao no array commits */
    for (j = 0; j < tamTabela; j++){
        if (tabela->linhaTabela[j].op == 'C'){
            commits[totalCommits] = tabela->linhaTabela[j].transacao;
            deuCommit = 1;
            totalCommits++;
        }
        /* Se depois de achar operacao commit não achar mais, testa os algoritmos*/
        if (deuCommit && (tabela->linhaTabela[j].op != 'C')){
            if(verificaConflito(tabela,tamTabela,commits,totalCommits,matriz,qntAtributos,vetorAtributos)){
                strcpy(serial,"SS");
            }
            else{
                strcpy(serial,"NS");
            }
            if(verificaEquivalencia(tabela,tamTabela,commits,totalCommits,qntAtributos,vetorAtributos,ini)){
                strcpy(equivalente,"SV");
            }
            else{
                strcpy(equivalente,"NV");
            }
            
            printf("%d ",saida);
            qsort(commits,totalCommits,sizeof(int), comparaCrescente);
            for(i = 0; i < totalCommits ; i++){
                printf("%d",commits[i]);
                if(i != totalCommits - 1){
                    printf(",");
                }
            }
            printf(" %s %s\n",serial,equivalente);
            deuCommit = 0;
            totalCommits = 0;
            memset(commits, 0 , sizeof(commits));
            saida++;
            ini = j;

        }

    }
    
    if(tabela->linhaTabela[j - 1].op == 'C'){
        if(verificaConflito(tabela,tamTabela,commits,totalCommits,matriz,qntAtributos,vetorAtributos)){
            strcpy(serial,"SS");
        }
        else{
            strcpy(serial,"NS");
        }
        if(verificaEquivalencia(tabela,tamTabela,commits,totalCommits,qntAtributos,vetorAtributos,ini)){
            strcpy(equivalente,"SV");
        }
        else{
            strcpy(equivalente,"NV");
        }
        
        printf("%d ",saida);
        qsort(commits,totalCommits,sizeof(int), comparaCrescente);
        for(i = 0; i < totalCommits ; i++){
            printf("%d",commits[i]);
            if(i != totalCommits - 1){
                printf(",");
            }
        }
        printf(" %s %s\n",serial,equivalente);
        memset(commits,0 , sizeof(commits));
    }
}


void liberaTudo(tabelaT* tabela){
    free(tabela->linhaTabela);
    free(tabela);
}