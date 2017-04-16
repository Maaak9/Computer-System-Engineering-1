#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H


typedef struct node {
	int xCord; //4
        int yCord;
	struct node *next;
}node;

typedef struct linkedlist {
	int size;
	node *head;	
	node *tail;
}linkedlist;



extern linkedlist *listan;
//extern node nodetemp;

int AddLast(int xCord, int yCord , linkedlist *lista);
void linkedlistInit(linkedlist *lista);
void printLinkedList(linkedlist *lista);
void removeFirst(linkedlist *lista);
int addhead(int xCord, int yCord , linkedlist *lista);
void removeAll(linkedlist *lista);

#endif