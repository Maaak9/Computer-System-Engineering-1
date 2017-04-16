#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Dlinkedlist.h"
#include "Main.h"

int AddLast(int xCord, int yCord , linkedlist *lista) {
	node * nodetemp = (node *)malloc(sizeof(node));
	if (nodetemp == NULL) {
		printf("Error: out of memory\n");
		return -1;
	}
	else {
		nodetemp->xCord = xCord;
                nodetemp->yCord = yCord;
		nodetemp->next = NULL;
		if (lista->size == 0) {
			lista->head->next = nodetemp;
			lista->tail = nodetemp;
			lista->size += 1;
		}
		else {
			lista->tail->next = nodetemp;
			lista->tail = nodetemp;
			lista->size += 1;
		}
		return 1;
	}
}

void linkedlistInit(linkedlist *lista) {
	node * nodetemp = (node *)malloc(sizeof(node));
	nodetemp->xCord = 0;
        nodetemp->yCord = 0;
	nodetemp->next = NULL;
	lista->head = nodetemp;
	lista->tail = NULL;
	lista->size = 0;
}

void printLinkedList(linkedlist *lista) {
	node * nodetemp;
	nodetemp = lista->head->next;
	for (int i = 0; i < lista->size; i++) {
		printf("xCord\n %d \n", nodetemp->xCord);
                printf("yCord\n %d \n", nodetemp->yCord);
		nodetemp = nodetemp->next;
	}
}

void removeFirst(linkedlist *lista) {
	node *nodetemp;
	nodetemp = lista->head->next;
	lista->head->next = lista->head->next->next;
	free(nodetemp);
	lista->size -= 1;
}

void removeAll(linkedlist *lista){
  node *nodetemp;
  while(lista->size != 0){
    nodetemp = lista->head->next;
    lista->head->next = lista->head->next->next;
    free(nodetemp);
    lista->size -= 1;
  }
}

int addhead(int xCord, int yCord , linkedlist *lista) {
  
	node * nodetemp = (node *)malloc(sizeof(node));
	if (nodetemp == NULL) {
		printf("Error: out of memory\n");
		return -1;
	}
	else {
		nodetemp->xCord = xCord;
                nodetemp->yCord = yCord;
		nodetemp->next = NULL;
		nodetemp->next = lista->head->next;
		lista->head->next = nodetemp;
		lista->size += 1;
		return 1;
	}
  
}


