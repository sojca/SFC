#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "vector_editor.h"


GtkGrid *grid = NULL;

double radius = 1.0;

int ndimensions = 0;
int nvectors = 0;

extern I_VECTORS vectors;


int create_vector_editor(){
	
	GtkBuilder *builder = NULL;
	GtkWidget *dialog = NULL;

	int i,j;
	int response_id;
	// GtkSpinButton *sd;
	// GtkSpinButton *vd;
	// int class;
	// GtkAdjustment *vector_adjustment;
	
	
	builder = gtk_builder_new_from_file("glade/editor.glade");
	gtk_builder_connect_signals(builder, NULL);
	
	dialog = GTK_WIDGET(gtk_builder_get_object (builder, "dialog1"));
	// sd = GTK_SPIN_BUTTON(gtk_builder_get_object (builder, "dimension_spinbutton"));
	// vd = GTK_SPIN_BUTTON(gtk_builder_get_object (builder, "vector_spinbutton"));
	
	// if(vectors == NULL){
	grid = GTK_GRID(gtk_builder_get_object (builder, "vectors_grid"));

	radius = get_radius(&vectors);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object (builder, "radius_spinbutton")), radius);

	ndimensions = get_dimensions(&vectors);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object (builder, "dimension_spinbutton")), ndimensions);
	
	nvectors = size_of_input_vector_list(&vectors);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object (builder, "vector_spinbutton")), nvectors);
	
	for(i = 1; i <= ndimensions; i++){
		create_dimension_label(i);
	}

	i = 1;
	select_first_input_vector(&vectors);
	while(is_active_input_vector(&vectors)){

		create_vector_label(i);
	
		create_class_spinbutton(1, i, get_class(&vectors, i));

		for(j = 1; j <= ndimensions; j++){
			create_vector_spinbutton(j+1, i, get_value(&vectors, i, j));
		}

		select_next_input_vector(&vectors);
		i++;
	}

	g_object_unref(builder);
	response_id = gtk_dialog_run(GTK_DIALOG(dialog));
	
	gtk_widget_destroy(dialog);

	return response_id;
}
		
void add_dimension() {	

	int i, j;
	int *dimension;

	dimension = malloc(sizeof(int)*nvectors);
	memset(dimension, 0, sizeof(int)*nvectors);

	for(i = get_dimensions(&vectors)+1; i <= ndimensions; i++){
		create_dimension_label(i);

		for(j = 1; j <= nvectors; j++){
			create_vector_spinbutton(i+1, j, 0);
		}

		insert_dimension(&vectors, dimension);

	}

	free(dimension);
}

void del_dimension() {	

	int i = 0;

	for(i = get_dimensions(&vectors); i > ndimensions; i--){
		gtk_grid_remove_column(grid, i+1);
		dispose_dimension(&vectors);
	}
}

void add_vector() {	

	int i,j;
	int *vect;

	vect = malloc(sizeof(int)*ndimensions);
	memset(vect, 0, sizeof(int)*ndimensions);
	
	for(i = size_of_input_vector_list(&vectors) + 1; i <= nvectors; i++){
		create_vector_label(i);
		create_class_spinbutton(1, i, 1);

		for(j = 2; j <= ndimensions+1; j++){
			create_vector_spinbutton(j, i, 0);
		}

		insert_last_input_vector(&vectors, vect, 1);
	}

	free(vect);
}


void del_vector() {	
	int i = 0;

	for(i = size_of_input_vector_list(&vectors); i > nvectors; i--){
		gtk_grid_remove_row(grid, i);
		dispose_last_input_vector(&vectors);
	}
}

void dimensions_value_changed(GtkSpinButton *spin_button, gpointer user_data){
		
	int new_dimensions = gtk_spin_button_get_value_as_int(spin_button);
	
	if(new_dimensions > ndimensions){
		ndimensions = new_dimensions;
		add_dimension();
	}
	else if(new_dimensions < ndimensions){
		ndimensions = new_dimensions;
		del_dimension();
	}
}

void vectors_value_changed(GtkSpinButton *spin_button, gpointer user_data){
		
	int new_vectors = gtk_spin_button_get_value_as_int(spin_button);
	
	if(new_vectors > nvectors){
		nvectors = new_vectors;
		add_vector();
	}
	else if(new_vectors < nvectors){
		nvectors = new_vectors;
		del_vector();
	}
}

void radius_value_changed(GtkSpinButton *spin_button, gpointer user_data){
		
	set_radius(&vectors, gtk_spin_button_get_value(spin_button));
}


void class_value_changed(GtkSpinButton *spin_button, DIMENSION* user_data){
		
	set_class(&vectors, user_data->t, gtk_spin_button_get_value(spin_button));
}

void vector_value_changed(GtkSpinButton *spin_button, DIMENSION* user_data){

	set_value(&vectors, user_data->t, user_data->l, gtk_spin_button_get_value(spin_button));
}


void create_class_spinbutton(int left, int top, int value) {

	GtkAdjustment *class_adjustment;
	GtkWidget *class;

	class_adjustment = gtk_adjustment_new(0, 1, 100, 1, 10, 0);
	class = gtk_spin_button_new(class_adjustment, 1, 0);
	gtk_grid_attach(grid, class, left, top, 1, 1);
	g_signal_connect(G_OBJECT(class), "value-changed", G_CALLBACK(class_value_changed), (gpointer) create_dimension(left, top));

	gtk_spin_button_set_value(GTK_SPIN_BUTTON(class), value);

	gtk_widget_show(class);
}

void create_vector_spinbutton(int left, int top, int value) {

	GtkAdjustment *vector_adjustment;
	GtkWidget *input;

	vector_adjustment = gtk_adjustment_new(0, -100, 100, 1, 10, 0);
	input = gtk_spin_button_new(vector_adjustment, 0, 0);
	gtk_grid_attach(grid, input, left, top, 1, 1);	
	g_signal_connect(G_OBJECT(input), "value-changed", G_CALLBACK(vector_value_changed), (gpointer) create_dimension(left-1, top));

	gtk_spin_button_set_value(GTK_SPIN_BUTTON(input), value);

	gtk_widget_show(input);
}


DIMENSION* create_dimension (int left, int top) {
	
	DIMENSION *d;
	d = malloc(sizeof(DIMENSION));	
	d->l = left;
	d->t = top;

	return d;
}

void create_dimension_label(int left){

	GtkWidget *label;	
	char *label_text = (char*)malloc(42 * sizeof(char));
	
	sprintf(label_text, "Dimension %i", left);
	label = gtk_label_new(label_text);
	
	gtk_grid_attach(grid, label, left+1, 0, 1, 1);
	gtk_widget_show(label);	

	free(label_text);
}

void create_vector_label(int top) {

	GtkWidget *label;	
	char *label_text = (char*)malloc(42 * sizeof(char));
	
	sprintf(label_text, "Vector %i", top);
	label = gtk_label_new(label_text);
	
	gtk_grid_attach(grid, label, 0, top, 1, 1);
	gtk_widget_show(label);	

	free(label_text);
}
