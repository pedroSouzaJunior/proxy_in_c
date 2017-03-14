#ifndef _CACHE_H
#define _CACHE_H

#include<stdio.h>
#include<stdlib.h>

int tamanho_max_MB;
int tamanho_atual_MB;

No* lista;
// hashtable* hashtable;

/*
* Insere um dado na cache
*/
void inserir(char[] url, int tamanho, char* dados)
{

    /* se tem espaço livre, adiciona */
    if((espaco_livre() - tamanho) > 0)
    {
        Hashtable* ponteiro = hashtable->inserir(url, dados);
        lista->inserir(ponteiro);
        tamanho_atual_MB = tamanho_atual_MB + tamanho;
    }
    else 
    {
        Hashtable* no = lista->removerLRU();
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