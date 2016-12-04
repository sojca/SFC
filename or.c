#include <stdio.h>
#include <stdlib.h>

#include "or.h"


void init_ors_list (ORS *L) {

	L->Active = NULL;
	L->First = NULL;	
	L->Last = NULL;	
}

void dispose_ors_list (ORS *L) {

	OR ptr, delptr;
	ptr = L->First;
	
	while(ptr != NULL)
	{
		delptr = ptr;
		ptr = ptr->next;
		free(delptr);
	}	

	L->Active = NULL;
	L->First = NULL;
	L->Last = NULL;
}

void insert_first_or (ORS *L, int class) {

	OR ptr = malloc(sizeof(struct or));
	
	if(ptr == NULL)
		fprintf(stderr, "Malloc err\n");
		// Error();

	ptr->class = class;
	ptr->next = L->First;
	ptr->prev = NULL;

	if(L->First == NULL)
		L->Last = ptr;
	else
		L->First->prev = ptr;
	L->First = ptr;
}

void insert_last_or (ORS *L, int class) {

	OR ptr = malloc(sizeof(struct or));
	
	if(ptr == NULL)
		fprintf(stderr, "Malloc err\n");
		// Error();

	ptr->class = class;
	ptr->next = NULL;
	ptr->prev = L->Last;

	if(L->Last == NULL)
		L->First = ptr;
	else
		L->Last->next = ptr;
	L->Last = ptr;
}

void select_first_or(ORS *L) {

	L->Active = L->First;
}

void select_last_or(ORS *L) {

	L->Active = L->Last;
}

void select_next_or(ORS *L){
	
	if(L->Active != NULL)
		L->Active = L->Active->next;
}

int is_active_or(ORS *L){
	
	return (L->Active != NULL)?TRUE:FALSE;	
}

int is_exist_or_with_class(ORS *L, int class){
	
	OR ptr = L->First;
	while(ptr != NULL){

		if(class == ptr->class)
			return TRUE;
		ptr = ptr->next;
	}
	return FALSE;	
}

OR get_or_with_class(ORS *L, int class){
	
	OR ptr = L->First;
	while(ptr != NULL){

		if(class == ptr->class)
			return ptr;
		ptr = ptr->next;
	}
	return NULL;	
}

int size_of_ors_list (ORS *L) {

	OR ptr = L->Active;
	int count = 0;

	select_first_or(L);
	while(is_active_or(L)){
		count++;
		select_next_or(L);
	}

	L->Active = ptr;
	return count;
}
