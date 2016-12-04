#ifndef HYPERSPHERE_H
#define HYPERSPHERE_H

#include "input.h"
#include "or.h"

// #define TRUE 1
// #define FALSE 0

typedef struct hypersphere
{
	double radius;
	int class;

	OR or_layer;
	int *i;

	struct hypersphere *next;
	struct hypersphere *prev;
} *HYPERSPHERE;


typedef struct { 
    HYPERSPHERE Active;
    HYPERSPHERE First;
    HYPERSPHERE Last;
} HYPERSPHERES;


void init_hyperspheres_list (HYPERSPHERES *);
void dispose_hyperspheres_list (HYPERSPHERES *);
void insert_first_hypersphere (HYPERSPHERES *, double, int, int *, int);
void insert_last_hypersphere (HYPERSPHERES *, double, int, int *, int);
void select_first_hypersphere (HYPERSPHERES *);
void select_last_hypersphere (HYPERSPHERES *);
void select_next_hypersphere(HYPERSPHERES *);
int is_active_hypersphere(HYPERSPHERES *);
int size_of_hypersphere_list(HYPERSPHERES *);

#endif
