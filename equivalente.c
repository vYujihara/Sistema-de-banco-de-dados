#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libEscalona.h"
#include "equivalente.h"
#include "seriabilidade.h"


/* após cada permutacao, faz uma tabela de visão */
void completaVisoes(tabelaT* tabela,tabelaT* visoes, int commits[], int totalCommits, int visaoIndice,int tamTabela){
    int i, j;
    int linhaAtual = 0;

    for(i = 0; i <= totalCommits ; i++){
        for(j = 0; j < tamTabela ; j++){
            if((tabela->linhaTabela[j].transacao == commits[i]) && (tabela->linhaTabela[j].op != 'C')){
                visoes[visaoIndice].linhaTabela[linhaAtual].atributo = tabela->linhaTabela[j].atributo;
                visoes[visaoIndice].linhaTabela[linhaAtual].op = tabela->linhaTabela[j].op;
                visoes[visaoIndice].linhaTabela[linhaAtual].transacao = tabela->linhaTabela[j].transacao;
                visoes[visaoIndice].linhaTabela[linhaAtual].timestamp = linhaAtual + 1;
                linhaAtual++;
            }
        }
    }

}

void troca(int* a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;

}

/* permuta todos as transacoes concorrentes */
void permutacao(tabelaT* tabela,tabelaT* visoes, int commits[], int indice, int n, int* visaoIndice, int tamTabela,int tamVisao){
    int i;

    if (indice == n){
        completaVisoes(tabela,visoes,commits, n, *visaoIndice,tamTabela);
        (*visaoIndice)++;
    }
    else{
        for (i = indice; i <= n ; i++){
            troca((commits + indice), (commits + i) );
            permutacao(tabela,visoes,commits, indice + 1, n,visaoIndice,tamTabela,tamVisao);
            troca((commits + indice), (commits + i) );
        }
    }
}

int retornaTamanhoVisao(tabelaT* tabela, int tamTabela,int commits[], int totalCommits){
    int i,j;
    int tam=0;
    
    for(i = 0; i < tamTabela ; i++){
        for(j = 0; j < totalCommits ; j++){
            if((tabela->linhaTabela[i].transacao == commits[j]) && (tabela->linhaTabela[i].op != 'C')){
                tam++;
            }
        }
    }
    return tam;
}

// funcao fatorial recursiva
int fat (int n) {

if ((n==1) || (n==0)) 
    return 1;               
else
    return fat(n-1)*n;
}

/* Guarda ti, tj e atributo da regra leitura apos escrita em vetorRegras */
void adicionaRegra(regrasT* vetorRegras, int contVetorRegras, int ti, int tj,char atributo){
    vetorRegras[contVetorRegras].ti = ti;
    vetorRegras[contVetorRegras].tj = tj;
    vetorRegras[contVetorRegras].atributo = atributo;
}


/* Testa a regra leitura após a escrita */
int testaRegra(tabelaT* tabela,char op, char atributo, int* tj, int* ti, int k,int commits[],int totalCommits){
    int i;
    int isTransacao = 0;


    /* Verifica se a transacao esta entre os que deram commit */
    for(i = 0; i < totalCommits; i++){
        if(tabela->linhaTabela[k].transacao == commits[i]) {
            isTransacao = 1;
        }
    }
    if(!isTransacao){
        return 0;
    }
    
    if ((tabela->linhaTabela[k].op) == op && (tabela->linhaTabela[k].atributo == atributo) && (tabela->linhaTabela[k].transacao != *tj)){
        
        *ti = tabela->linhaTabela[k].transacao ;
        return 1;
    }
    return 0;

}

/* Se retornar 1, Tj faz parte das transacoes que deram commit*/
int verificaTj(int commits[], int totalCommits,int tj){
    int i;
    int isTransacao = 0;

    /* Verifica se a transacao esta entre os que deram commit */
    for(i = 0; i < totalCommits; i++){
        if(tj == commits[i]){
            isTransacao = 1;
            
        }
    }
    if(!isTransacao){
        return 0;
    }
    return 1;
}

/* guarda a escrita no vetor lastEscrita(guarda a ultima escrita, cada indice representa um atributo)*/
void ultimaEscrita(tabelaT* tabela,int lastEscrita[], int commits[], int totalCommits,int tj, int indice){
    int i;
    int isTransacao = 0;

    /* Verifica se a transacao esta entre os que deram commit */
    for(i = 0; i < totalCommits; i++){
        if(tj == commits[i]){
            isTransacao = 1;
            
        }
    }
    if(!isTransacao){
        return ;
    }

    lastEscrita[indice] = tj;
}

int ehEquivalente(tabelaT* visoes, int nVisoes,int tamVisao ,equivalenciaT* equivalente, regrasT* vetorRegras, int contVetorRegras,int lastEscrita[], char vetorAtributos[],int qntAtributos){
    int i, j,k, l;
    int passouRegra;
    char atributo;
    int ultimaEscrita = -1;

    for(i = 0; i < nVisoes; i++){
         /* testa todas as regras de leitura apos escrita, se passar todas nao faz nada, ja que equivalente[i].leitura ja contem 1 */
        for(j = 0; j < contVetorRegras ; j++){
            passouRegra = 0;
            for(k = 0; k < tamVisao; k++){
                if((visoes[i].linhaTabela[k].op == 'W') && (visoes[i].linhaTabela[k].transacao == vetorRegras[j].tj) && (visoes[i].linhaTabela[k].atributo = vetorRegras[j].atributo)){
                    for(l = 0; l < k ; l++){
                        if((visoes[i].linhaTabela[l].op == 'R') && (visoes[i].linhaTabela[l].transacao == vetorRegras[j].ti) && (visoes[i].linhaTabela[l].atributo = vetorRegras[j].atributo)){
                            passouRegra = 1;       
                        }
                    }
                }
            }
            /* se nao passou em pelo menos uma regra, sai do loop e testa outra visao*/
            if (!passouRegra){
                equivalente[i].leitura = 0;
                break;
            }
        }
    
    }
    /* teste das ultimas escritas nas visoes */
    for (i = 0 ; i < nVisoes ; i++){
        for (j = 0 ; j < qntAtributos; j++){
            atributo = vetorAtributos[j];
            for(k = 0; k < tamVisao; k++){
                if((visoes[i].linhaTabela[k].op == 'W') && (visoes[i].linhaTabela[k].atributo == atributo)){
                    ultimaEscrita = visoes[i].linhaTabela[k].transacao;
                }
            }
            /* cada posicao de lastEscrita representa um atributo, se a ultima escrita ocorrer na mesma transacao da visão, é aceita a regra*/
            if(lastEscrita[j] == ultimaEscrita){
                equivalente[i].ultimaEscrita = 1;
            }
        }
    }

    /* Se aceitar as 2 regras, é SV */
    for(i = 0 ; i < nVisoes; i++){
        if(equivalente[i].leitura && equivalente[i].ultimaEscrita){
            return 1;
        }
    }
    return 0;
}

void freeAll(tabelaT* visoes,int nVisoes, equivalenciaT* equivalente, regrasT* vetorRegras){
    for(int i=0 ; i < nVisoes ; i++){
        free(visoes[i].linhaTabela);
    }
    free(visoes);
    free(equivalente);
    free(vetorRegras);
}

/* verifica se a tabela é igual a visão, se é a transação original é serial e assim não tem visão */
int igual(int ini, tabelaT* tabela, tabelaT* visoes, int tamVisao,int nVisoes){
    int i, j;
    int ehIgual = 1;

    for(i=0; i < nVisoes ; i++){
        for(j=0; j < tamVisao ; j++){
            if(tabela->linhaTabela[ini+j].transacao != visoes[i].linhaTabela[j].transacao){
                ehIgual = 0;
            }
        }
        if(ehIgual){
            return 1;
        }
        ehIgual  = 1;
    }
    return 0;
}

int verificaEquivalencia(tabelaT* tabela, int tamTabela, int commits[],int totalCommits,int qntAtributos,char vetorAtributos[], int ini){
    tabelaT* visoes;
    int nVisoes;
    int i,j,k;
    int visaoIndice = 0;
    int tamVisao = 0;
    char atributo;
    int tj = 0;
    int ti = 0;
    equivalenciaT* equivalente;
    regrasT* vetorRegras;
    int contVetorRegras = 0;
    int lastEscrita[qntAtributos];

    nVisoes=fat(totalCommits);
    visoes = malloc(sizeof(tabelaT) * nVisoes);
    equivalente = malloc(sizeof(equivalenciaT) * nVisoes);
    vetorRegras = malloc(sizeof(regrasT) * 50);
    tamVisao = retornaTamanhoVisao(tabela,tamTabela,commits,totalCommits);
    for(i = 0; i < nVisoes ; i++){
        visoes[i].linhaTabela= malloc(sizeof(linhaT) * tamVisao);
        equivalente[i].leitura = 1;
        equivalente[i].ultimaEscrita = 0;
    }

    permutacao(tabela,visoes, commits, 0 , totalCommits -1,&visaoIndice, tamTabela,tamVisao);
    /* verifica se a tabela é igual a uma das visões */
    if(igual(ini,tabela,visoes,tamVisao,nVisoes)){
        return 0;
    }

    /* Testando as visoes */
    for(i = 0; i < qntAtributos; i++){
        atributo = vetorAtributos[i];
        for(j = 0; j < tamTabela; j++){
            if(tabela->linhaTabela[j].op == 'W' && (atributo == tabela->linhaTabela[j].atributo) ){
                tj = tabela->linhaTabela[j].transacao ; 
                if(!verificaTj(commits,totalCommits,tj)){
                    continue;
                }
                ultimaEscrita(tabela,lastEscrita,commits,totalCommits,tj,i);
                for (k = 0; k < j ; k++){
                    if(testaRegra(tabela,'R',atributo,&tj,&ti,k,commits,totalCommits)){
                        adicionaRegra(vetorRegras,contVetorRegras,ti,tj,atributo);
                        contVetorRegras++;
                    }
                }
            }
        }
    }

    if(ehEquivalente(visoes,nVisoes,tamVisao,equivalente,vetorRegras,contVetorRegras,lastEscrita,vetorAtributos,qntAtributos)){
        freeAll(visoes,nVisoes,equivalente,vetorRegras);
        return 1;
    }
    freeAll(visoes,nVisoes,equivalente,vetorRegras);
    return 0;


}