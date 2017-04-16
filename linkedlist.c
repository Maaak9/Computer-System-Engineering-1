#include <stdio.h>
#include <stdlib.h>
#include "at91sam3x8.h"

struct c_list {
    int nInfo;			/* Storage in node is a integer */
    struct c_list *next;	/* pointer to next node in linked list */
};

struct c_list *alloc_obj(int nData);
void print_list(struct c_list *prTemp);
void print_list2(struct c_list *prTemp);
void print_list2bw(struct c_list *prTemp);
void put_at_tail(struct c_list **prRefList, struct c_list *prObj);

struct c_list *alloc_obj(int nData)
{
    /* Pointer to a list object */
    struct c_list *prListObj;

    /* Allocate memory for a list object */
    prListObj = (struct c_list *) calloc(1 , sizeof(struct c_list));

    /* In case memory cant be allocated exit, else assign data and return pointer */
    if (prListObj == NULL) {
	printf("Error: out of memory\n");
	exit(-1);
	return NULL;
    } else {
	prListObj->nInfo = nData;
	prListObj->next = NULL;
	return prListObj;
    }
}


void print_list(struct c_list *prTemp)
{
    struct c_list *prTemp2;
    /* Operate on the temporary pointer not on the list */
    prTemp2 = prTemp;
    while (prTemp2 != NULL) {
	printf("%d\n", prTemp2->nInfo);
	prTemp2 = prTemp2->next;
    }
}

void print_list2(struct c_list *prTemp)
{
    /* printing the list can be made recursive (not that it is recommended ) */
    if (prTemp != NULL) {
	printf("%d\n", prTemp->nInfo);
	print_list2(prTemp->next);
    }
}

void print_list2bw(struct c_list *prTemp)
{
    /* printing the list can be made recursive (not that it is recommended ) */
    if (prTemp != NULL) {
	print_list2bw(prTemp->next);
	printf("%d\n", prTemp->nInfo);
    }
}

void put_at_tail(struct c_list **prRefList, struct c_list *prObj)
{
    /* Temporary pointer at first node in list */
    struct c_list *prTemp = *prRefList;

    if (*prRefList != NULL) {
	/* if list exist */
	while (prTemp->next != NULL) {
	    /* Traverse the list to the end */
	    prTemp = prTemp->next;
	}
	/* We have found the end of the list */
	prTemp->next = prObj;	/* Connect the last node */
    } else {
	/* List was empty, so the new list _is_ the node provided */
	*prRefList = prObj;
    }
}
