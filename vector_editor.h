#ifndef VECTOR_EDITOR_H
#define VECTOR_EDITOR_H

#include "vector_editor.h"

typedef struct dimension {
	int l;
	int t;
} DIMENSION;


void		create_vector_editor();
int			warn_dialog(GtkWidget*);
void 		create_class_spinbutton(int, int, int);
void 		create_vector_spinbutton(int, int, int);

void 		create_vector_label(int);
void 		create_dimension_label(int);

DIMENSION* 	create_dimension (int, int );

#endif
