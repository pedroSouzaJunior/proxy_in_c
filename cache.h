#ifndef _CACHE_H
#define _CACHE_H

#include<stdio.h>
#include<stdlib.h>
#include "list.h"





struct cache{

    int tamanho_max_MB;
    int tamanho_atual_MB;
    // hashtable* hashtable;
};



/*
* Insere um dado na cache
*/
void inserir(cache* cache, char[] url, int tamanhoEmBytes, char* dados)
{

    /* se tem espaço livre, adiciona */
    if((espaco_livre() - tamanhoEmBytes) > 0)
    {
        Hashtable* ponteiro = hashtable->inserir(url, dados);
        insere
        /*lista->inserir(ponteiro);*/
        tamanho_atual_MB = tamanho_atual_MB + tamanhoEmBytes;
    }
    else 
    {
        Hashtable* no = lista.removerLRU();
        hashtable->remover(no->chave);
        tamanho_atual_MB = tamanho_atual_MB + no->tamanho;
    }

}

/*
* Remover um dado na cache
*/
void remover(char[] url)
{

}

int espaco_livre()
{
    return tamanho_max_MB - tamanho_atual_MB;
}


#endif 