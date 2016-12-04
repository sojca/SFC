#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hypersphere.h"


void init_hyperspheres_list (HYPERSPHERES *L) {

	L->Active = NULL;
	L->First = NULL;	
	L->Last = NULL;	
}

void dispose_hyperspheres_list (HYPERSPHERES *L) {

	HYPERSPHERE ptr, delptr;
	ptr = L->First;
	
	while(ptr != NULL)
	{
		delptr = ptr;
		ptr = ptr->next;
		free(delptr->i);
		free(delptr);
	}	

	L->Active = NULL;
	L->First = NULL;
	L->Last = NULL;	
}

void insert_first_hypersphere (HYPERSPHERES *L, double radius, int class, int *vector, int len) {

	HYPERSPHERE ptr = malloc(sizeof(struct hypersphere));
	if(ptr == NULL)
		fprintf(stderr, "Malloc err\n");

	ptr->i = malloc(sizeof(int)*len);
	if(ptr->i == NULL)
		fprintf(stderr, "Malloc err\n");


	memcpy(ptr->i, vector, sizeof(int)*len); 


	ptr->radius = radius;
	ptr->class = class;
	ptr->next = L->First;
	ptr->prev = NULL;

	if(L->First == NULL)
		L->Last = ptr;
	else
		L->First->prev = ptr;

	L->First = ptr;
}

void insert_last_hypersphere (HYPERSPHERES *L, double radius, int class, int *vector, int len) {

	HYPERSPHERE ptr = malloc(sizeof(struct hypersphere));
	if(ptr == NULL)
		fprintf(stderr, "Malloc err\n");

	ptr->i = malloc(sizeof(int)*len);
	if(ptr->i == NULL)
		fprintf(stderr, "Malloc err\n");


	memcpy(ptr->i, vector, sizeof(int)*len); 


	ptr->radius = radius;
	ptr->class = class;
	ptr->next = NULL;
	ptr->prev = L->Last;

	if(L->Last == NULL)
		L->First = ptr;
	else
		L->Last->next = ptr;
	L->Last = ptr;
}


void select_first_hypersphere(HYPERSPHERES *L) {
	L->Active = L->First;
}

void select_last_hypersphere(HYPERSPHERES *L) {

	L->Active = L->Last;
}

void select_next_hypersphere(HYPERSPHERES *L){

	if(L->Active != NULL)
		L->Active = L->Active->next;	
}

int is_active_hypersphere(HYPERSPHERES *L){

	return (L->Active != NULL)?TRUE:FALSE;	
}

int size_of_hypersphere_list(HYPERSPHERES *L) {

	HYPERSPHERE ptr = L->Active;
	int count = 0;

	select_first_hypersphere(L);
	while(is_active_hypersphere(L)){
		count++;
		select_next_hypersphere(L);
	}

	L->Active = ptr;
	return count;
}

