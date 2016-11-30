#ifndef INPUT_H
#define INPUT_H

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


typedef struct input
{
	int *i;
	int class;  

	struct input *next;
	struct input *prev;
} *I_VECTOR;


typedef struct { 
	int dimensions;
	double radius;
	    
    I_VECTOR Active;
    I_VECTOR First;
    I_VECTOR Last;
} I_VECTORS;


void init_input_vector_list (I_VECTORS *, int, double);
int get_dimensions(I_VECTORS *);
double get_radius(I_VECTORS *);
void set_radius(I_VECTORS *, double);

void set_value(I_VECTORS *, int, int, int);
int get_value(I_VECTORS *, int, int);
void set_class(I_VECTORS *, int, int);
int get_class(I_VECTORS *, int);

void dispose_last_input_vector (I_VECTORS *L);
void insert_dimension(I_VECTORS *, int *);
void dispose_dimension(I_VECTORS *);

void dispose_input_vector_list (I_VECTORS *);

void insert_first_input_vector (I_VECTORS *, int *, int);
void insert_last_input_vector (I_VECTORS *, int *, int);
void select_first_input_vector (I_VECTORS *);
void select_next_input_vector(I_VECTORS *);
int size_of_input_vector_list(I_VECTORS *);
int is_active_input_vector(I_VECTORS *);

#endif
