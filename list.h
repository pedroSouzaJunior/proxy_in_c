#ifndef _LIST_H
#define _LIST_H

/* Doubly Linked List implementation */
#include<stdio.h>
#include<stdlib.h>

struct No  {
	int dado;
	 No* prox;
	 No* prev;
};

struct No* cabeca; // global variable - pointer to head node.


//Creates a new Node and returns pointer to it. 
struct No* criaNo(int x) {
	struct No* novoNo = (struct No*)malloc(sizeof(struct No));
	novoNo->dado = x;
	novoNo->prev = NULL;
	novoNo->prox = NULL;
	return novoNo;
}

//Inserts a Node at head of doubly linked list
void insereCabeca(int x) {
	struct No* novoNo = criaNo(x);
	if(cabeca == NULL) {
		cabeca = novoNo;
		return;
	}
	cabeca->prev = novoNo;
	novoNo->prox = cabeca; 
	cabeca = novoNo;
}

//Inserts a Node at tail of Doubly linked list
void insereCauda(int x) {
	struct No* temp = cabeca;
	struct No* novoNo = criaNo(x);
	if(cabeca == NULL) {
		cabeca = novoNo;
		return;
	}
	while(temp->prox != NULL) temp = temp->prox; // Go To last Node
	temp->prox = novoNo;
	novoNo->prev = temp;
}

//Prints all the elements in linked list in forward traversal order
void Print() {
	struct No* temp = cabeca;
	printf("Forward: ");
	while(temp != NULL) {
		printf("%d ",temp->dado);
		temp = temp->prox;
	}
	printf("\n");
}

No* removerLRU(){
	No* temp = list->LRU;
	list->LRU = list->prox;
	cabeca = list->prox;
	return temp;
}


#endif