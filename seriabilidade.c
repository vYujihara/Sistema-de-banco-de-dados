#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "libEscalona.h"
#include "equivalente.h"
#include "seriabilidade.h"


/* Busca em profundidade 
    Solucao: para saber se tem ciclo, devemos trackear  as arestas que estão atualemnte na recursao da pilha da busca em profundidade
    se de alguma forma atingirmos uma aresta que está na pilha de recursão, entao dizemos que tem um ciclo */


int ehCicloAux(int matriz[M][M] ,int v , int nodosVisitados[], int nodosProcessados[], int totalCommits){
    int i;
    
    if (!nodosVisitados[v]){
        nodosVisitados[v] = 1;
        nodosProcessados[v] = 1;

    }

    for(i = 0; i < totalCommits ; i++) {
        if (matriz[v][i] && nodosProcessados[i]){
            return 1;
        }
        if (matriz[v][i] && ehCicloAux(matriz, i , nodosVisitados, nodosProcessados, totalCommits)){
            return 1 ; 
        }
    }
    nodosProcessados[v] = 0;
    return 0;
}

int ehCiclo(int matriz[M][M], int totalCommits){
    int nodosVisitados[totalCommits];
    int nodosProcessados[totalCommits];
    int i;

    for (i = 0 ; i < totalCommits ; i++){
        nodosVisitados[i] = 0;
        nodosProcessados[i] = 0 ;

    }

    for ( i = 0; i < totalCommits ; i++){
        if (ehCicloAux(matriz,i,nodosVisitados, nodosProcessados,totalCommits)){
            return 1;
        }
    }
    return 0 ;
}




/* pega os indices da matriz e armazena nas variáveis linha e coluna */
void pegaIndice(int commits[],int totalCommits, int ti, int tj,int* linha, int* coluna){
    int k;

    for (k = 0; k < totalCommits ; k++){
        if( (commits[k]) == ti){
            *linha = k;
        }
        if( (commits[k]) == tj){
            *coluna = k;
        }
    }
}


int verificaConflito(tabelaT* tabela, int tamTabela, int commits[],int totalCommits,int matriz[M][M],int qntAtributos,char vetorAtributos[]){
    int tj = 0;
    int ti = 0;
    int linha = 0;
    int coluna = 0;
    int i, j, k , l;
    int transacaoCerta;
    char atributo;
    
    memset(matriz,0,sizeof(int) * M * M);
    /* para cada atributo, vai criar um grafo representado por matriz */
    for(l = 0; l < qntAtributos ; l++){
        memset(matriz,0,sizeof(int) * M * M);
        atributo = vetorAtributos[l];
        /* Percorre a tabela */
        for(j = 0; j < tamTabela; j++){
            transacaoCerta = 0;

            /* verifica se a transacao faz parte das transacoes que deram commit */
            for (i = 0 ; i < totalCommits ; i++){
                if(tabela->linhaTabela[j].transacao == commits[i]){
                    transacaoCerta = 1;
                }
            }
            if (!transacaoCerta){
                continue;
            }

            /* Primeira regra */
            /* Se achou operacao R e é igual o atributo do ciclo aplica regra */
            if( (tabela->linhaTabela[j].op) == 'R' && (atributo == tabela->linhaTabela[j].atributo) ){
                tj = tabela->linhaTabela[j].transacao ;
    
                /* Percorre todas as operacoes antes da linha j*/
                for (k = 0; k < j ; k++){
                    /* Se leitura depois de uma escrita, cria aresta ti -> tj*/
                    if(ocorreuRegra(tabela, 'W', atributo, &tj, &ti, k,commits,totalCommits)){
                        pegaIndice(commits,totalCommits,ti,tj,&linha,&coluna);
                        matriz[linha][coluna]++;
                    }
                }

            }
            /* Segunda regra */
            if(tabela->linhaTabela[j].op == 'W' && (atributo == tabela->linhaTabela[j].atributo) ){
                tj = tabela->linhaTabela[j].transacao ;          
                for (k = 0; k < j ; k++){
                    /* Se escrita depois de uma leitura, cria aresta ti -> tj */
                    if(ocorreuRegra(tabela, 'R', atributo, &tj, &ti, k,commits,totalCommits)){
                        pegaIndice(commits,totalCommits,ti,tj,&linha,&coluna);
                        matriz[linha][coluna]++;
                    }
                }    
            }
            /* Terceira regra */
            if(tabela->linhaTabela[j].op == 'W' && (atributo == tabela->linhaTabela[j].atributo) ){
                tj = tabela->linhaTabela[j].transacao ;
                for (k = 0; k < j ; k++){
                    /* Se escrita depois de uma escrita, cria aresta ti -> tj */
                    if(ocorreuRegra(tabela, 'W', atributo, &tj, &ti, k,commits,totalCommits)){
                        pegaIndice(commits,totalCommits,ti,tj,&linha,&coluna);
                        matriz[linha][coluna]++;
                    }
                }
            }
        }

        if(ehCiclo(matriz,totalCommits)){
            return 0;
        }
        
    }
    return 1;
}   

int ocorreuRegra(tabelaT* tabela,char op, char atributo, int* tj, int* ti, int k,int commits[],int totalCommits){
    int i;
    int isTransacao = 0;

    /* Verifica se a transacao esta entre os que deram commit */
    for(i = 0; i < totalCommits; i++){
        if(tabela->linhaTabela[k].transacao == commits[i]){
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
