#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

void init_input_vector_list (I_VECTORS *L, int len, double r) {

	L->Active = NULL;
	L->First = NULL;	
	L->Last = NULL;	

	L->dimensions = len;
	L->radius = r;
}

double get_radius(I_VECTORS *L) {

	return L->radius;
}

int get_dimensions(I_VECTORS *L) {

	return L->dimensions;
}

void set_radius(I_VECTORS *L, double r) {

	L->radius = r;
}


void set_value(I_VECTORS *L, int vector, int dimension, int value){

	I_VECTOR ptr = L->Active;
	int i,j;

	i = 1;
	select_first_input_vector(L);
	while(is_active_input_vector(L)){

		if(vector == i++) {
			for(j = 1; j <= L->dimensions; j++){
				if(dimension == j){
					(L->Active)->i[dimension-1] = value;
					break;
				}
			}
			break;
		}

		select_next_input_vector(L);
	}
	select_first_input_vector(L);
	
	L->Active = ptr;
}

int get_value(I_VECTORS *L, int vector, int dimension) {

	I_VECTOR ptr = L->Active;
	int i,j;
	int value = -200;

	i = 1;
	select_first_input_vector(L);
	while(is_active_input_vector(L)){

		if(vector == i++) {
			for(j = 1; j <= L->dimensions; j++){
				if(dimension == j){
					value = (L->Active)->i[dimension-1];
					L->Active = ptr;
					return value;
				}
			}
			break;
		}

		select_next_input_vector(L);
	}
	select_first_input_vector(L);
	L->Active = ptr;

	return -200;
}

void set_class(I_VECTORS *L, int vector, int c) {
	
	I_VECTOR ptr = L->Active;
	int i = 1;

	select_first_input_vector(L);
	while(is_active_input_vector(L)){

		if(vector == i++) {
			(L->Active)->class = c;
		}

		select_next_input_vector(L);
	}

	select_first_input_vector(L);
	
	L->Active = ptr;
}

int get_class(I_VECTORS *L, int vector) {

	I_VECTOR ptr = L->Active;
	int i = 1;
	int class = -1;

	select_first_input_vector(L);
	while(is_active_input_vector(L)){
		if(vector == i++){
			class = (L->Active)->class;
			break;
		}

		select_next_input_vector(L);
	}

	L->Active = ptr;
	return class;
}


void dispose_input_vector_list (I_VECTORS *L) {

	I_VECTOR ptr, delptr;
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
	L->dimensions = 0;
}

void insert_first_input_vector (I_VECTORS *L, int *vector, int class) {

	I_VECTOR ptr = malloc(sizeof(struct input));
	
	if(ptr == NULL)
		fprintf(stderr, "Malloc err\n");


	ptr->i = malloc(sizeof(int)*(L->dimensions));
	
	if(ptr->i == NULL)
		fprintf(stderr, "Malloc err\n");


	memcpy(ptr->i, vector, sizeof(int)*(L->dimensions)); 
	ptr->class = class;
	ptr->next = L->First;
	ptr->prev = NULL;

	if(L->First == NULL)
		L->Last = ptr;
	else
		L->First->prev = ptr;
	L->First = ptr;

}

void insert_last_input_vector (I_VECTORS *L, int *vector, int class) {

	I_VECTOR ptr = malloc(sizeof(struct input));
	
	if(ptr == NULL)
		fprintf(stderr, "Malloc err\n");


	ptr->i = malloc(sizeof(int)*(L->dimensions));

	if(ptr->i == NULL)
		fprintf(stderr, "Malloc err\n");


	memcpy(ptr->i, vector, sizeof(int)*(L->dimensions)); 
	ptr->class = class;
	ptr->next = NULL;
	ptr->prev = L->Last;

	if(L->Last == NULL)
		L->First = ptr;
	else
		L->Last->next = ptr;
	L->Last = ptr;
}

void dispose_last_input_vector (I_VECTORS *L) {

	if(L->Last != NULL){
		I_VECTOR ptr = NULL;
		
		if(L->Last->prev != NULL){
			ptr = L->Last->prev;
			ptr->next = NULL;
		} 
		else {
			L->First = NULL;
		}

		free(L->Last->i);
		free(L->Last);

		L->Last = ptr;
	}
}

void insert_dimension(I_VECTORS *L, int *dimension) {
	int i;

	(L->dimensions)++;
	i = 0;

	select_first_input_vector(L);
	while(is_active_input_vector(L)){
		L->Active->i = realloc(L->Active->i, sizeof(int)*(L->dimensions));
		L->Active->i[(L->dimensions)-1] = dimension[i++];
		select_next_input_vector(L);
	}

}


void dispose_dimension(I_VECTORS *L) {
	
	if((L->dimensions) > 0){ 

		(L->dimensions)--;

		select_first_input_vector(L);
		while(is_active_input_vector(L)){
			L->Active->i[(L->dimensions)] = 0;
			select_next_input_vector(L);
		}
	}
}



void select_first_input_vector (I_VECTORS *L) {

	L->Active = L->First;
}

int size_of_input_vector_list (I_VECTORS *L) {

	I_VECTOR ptr = L->Active;
	int count = 0;

	select_first_input_vector(L);
	while(is_active_input_vector(L)){
		count++;
		select_next_input_vector(L);
	}

	L->Active = ptr;
	return count;
}

void select_next_input_vector(I_VECTORS *L) {

	if(L->Active != NULL){
		L->Active = L->Active->next;	
	}
}

int is_active_input_vector(I_VECTORS *L) {

	return (L->Active != NULL)?TRUE:FALSE;	
}
