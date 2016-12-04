#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#include "gui.h"
#include "vector_editor.h"

#include "hypersphere.h"
#include "input.h"
#include "or.h"
#include "rce.h"



static cairo_surface_t *surface, *old_surface;

pthread_mutex_t lock_rce = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_mem = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_rce = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

I_VECTORS vectors;
HYPERSPHERES hyperspheres;
ORS ors;

CONFIG_ITEM v_config;
CONFIG_ITEM h_config;
CONFIG_ITEM o_config;


gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	
	return FALSE;
}


void destroy_signal(GtkWidget *widget, gpointer data) {
	
	gtk_main_quit();
}


static void clear(GtkWidget *widget, gpointer data) {

	cairo_t *cr = cairo_create(surface);
	
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
	cairo_destroy(cr);
 
 	cr = cairo_create(old_surface);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
	cairo_destroy(cr);

	gdk_window_invalidate_rect(gtk_widget_get_window(data), NULL, FALSE);
}


gboolean next_rce_step(GtkWidget *widget, GdkEventButton *event, gpointer *data) {

	GtkWidget *toplevel;

	next_step();

	toplevel = gtk_widget_get_toplevel(widget);

	clear(toplevel, toplevel);
	gtk_widget_queue_draw (toplevel);



	draw_network();
	gtk_widget_queue_draw (toplevel);

	return TRUE;
}


gboolean clear_rce_drawing(GtkWidget *widget, GdkEventButton *event, gpointer *data) {
	
	GtkWidget *toplevel;
	
	toplevel = gtk_widget_get_toplevel(widget);
	dispose_ors_list(&ors);
	dispose_hyperspheres_list(&hyperspheres);

	clear(toplevel, toplevel);

	return TRUE;
}


gboolean stop_rce_step(GtkWidget *widget, GdkEventButton *event, gpointer *data) {
	
	return TRUE;
}


gboolean start_rce_step(GtkWidget *widget, GdkEventButton *event, gpointer data) {
	
	GtkWidget *toplevel;

	toplevel = gtk_widget_get_toplevel(widget);

	clear(toplevel, toplevel);
	gtk_widget_queue_draw (toplevel);

	draw_network();
	gtk_widget_queue_draw (toplevel);

	return TRUE;
}


gboolean open_vector_editor(GtkWidget *widget, GdkEventButton *event, gpointer *data) {
	
	create_vector_editor();

	config_items();	

	return TRUE;
}


static void copy(cairo_surface_t *dest, cairo_surface_t *source) {

	cairo_t *cr;

	cr = cairo_create(dest);
	cairo_set_source_surface (cr, source, 0, 0);
	cairo_paint (cr);
	cairo_destroy(cr);
}


gboolean draw_config(GtkWidget *widget, GdkEvent *event, gpointer data) {

	GtkAllocation allocation;
	int w;
	
	gtk_widget_get_allocation(widget, &allocation);
	
	w = (allocation.width > 800) ? allocation.width : 800;
	if(surface == NULL) {
		
		surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
				CAIRO_CONTENT_COLOR, w, 1500);
		old_surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
				CAIRO_CONTENT_COLOR, w, 1500);
		clear(widget, widget);
	}
	else {
		
		surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
				CAIRO_CONTENT_COLOR, w, 1500);

		cairo_t *cr;
		cr = cairo_create(surface);
		cairo_set_source_rgb(cr, 1, 1, 1);
		cairo_paint (cr);
		cairo_set_source_surface (cr, old_surface, 0, 0);
		cairo_paint (cr);
		cairo_destroy(cr);

		old_surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
			CAIRO_CONTENT_COLOR, w, 1500);
		cr = cairo_create(old_surface);
		cairo_set_source_rgb(cr, 1, 1, 1);
		cairo_paint (cr);
		cairo_set_source_surface (cr, surface, 0, 0);
		cairo_paint (cr);
		cairo_destroy(cr);

		gdk_window_invalidate_rect(gtk_widget_get_window(widget), NULL, FALSE);

	}

	return TRUE;
}


void config_items() {

	v_config.count = vectors.dimensions;
	h_config.count = size_of_hypersphere_list(&hyperspheres);
	o_config.count = size_of_ors_list(&ors);

	v_config.border = 100;
	h_config.border = 30;
	o_config.border = 30;
	
	v_config.px_to_add_x = 100; 
	h_config.px_to_add_x = 400;
	o_config.px_to_add_x = 600;

	v_config.radius = 10; 
	h_config.radius = 30;
	o_config.radius = 20;


	v_config.px_to_add_y = v_config.radius + v_config.border; 
	h_config.px_to_add_y = h_config.radius + h_config.border;
	o_config.px_to_add_y = o_config.radius + o_config.border;

	v_config.height = v_config.count*(v_config.radius*2 + v_config.border);
	h_config.height = h_config.count*(h_config.radius*2 + h_config.border);
	o_config.height = o_config.count*(o_config.radius*2 + o_config.border);
	
	if(v_config.height < h_config.height) {

		v_config.px_to_add_y += ABS(v_config.height-h_config.height)/2;
		o_config.px_to_add_y += ABS(o_config.height-h_config.height)/2;
	}
	else {

		h_config.px_to_add_y += ABS(v_config.height-h_config.height)/2;
		o_config.px_to_add_y += ABS(o_config.height-v_config.height)/2;
	}

	printf("vectors %i %i\n", v_config.count, v_config.px_to_add_y);
	printf("hypersp %i %i\n", h_config.count, h_config.px_to_add_y);
	printf("hypersp %i %i\n", o_config.count, o_config.px_to_add_y);
}


void draw_network() {
	

	cairo_t *cr;
	cr = cairo_create(surface);
	int v, h, o;
	char str[80];
	double middle_col = 0.9;
	double border_col = 0.2;
	double line = 0.2;
	

	cairo_set_line_width(cr, 5);
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);

  	cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 20);

	pthread_mutex_lock(&lock_mem);

	config_items();	

	for(v = 0; v < vectors.dimensions; v++) {

		cairo_set_source_rgb(cr, border_col, border_col, border_col);

		cairo_arc(cr, v_config.px_to_add_x, v_config.px_to_add_y + v*(v_config.radius*2 + v_config.border), v_config.radius, 0, 2 * M_PI);
		cairo_stroke_preserve(cr);
		
		cairo_set_source_rgb(cr, middle_col, middle_col, middle_col);
		cairo_fill(cr);

		sprintf(str, "i%i", v+1);
		cairo_set_source_rgb(cr, border_col, border_col, border_col);
		cairo_move_to(cr, v_config.px_to_add_x-50, v_config.px_to_add_y+7 + v*(v_config.radius*2 + v_config.border)); 
		cairo_show_text(cr, str);  
		cairo_stroke(cr);
		memset(str,'\0', 80);
	}

	o = 0;
	select_first_or(&ors);
	while(is_active_or(&ors)) {

		cairo_set_source_rgb(cr, border_col, border_col, border_col);

		cairo_arc(cr, o_config.px_to_add_x, o_config.px_to_add_y + o*(o_config.radius*2 + o_config.border), o_config.radius, 0, 2 * M_PI);
		cairo_stroke_preserve(cr);
		
		cairo_set_source_rgb(cr, middle_col, middle_col, middle_col);
		cairo_fill(cr);

		cairo_set_source_rgb(cr, border_col, border_col, border_col);
		cairo_move_to(cr, o_config.px_to_add_x-15, o_config.px_to_add_y+7 + o*(o_config.radius*2 + o_config.border)); 
  		cairo_show_text(cr, "OR");  
		cairo_stroke(cr);
		
		sprintf(str, "Class %i", ors.Active->class);
		cairo_set_source_rgb(cr, border_col, border_col, border_col);
		cairo_move_to(cr, o_config.px_to_add_x+50, o_config.px_to_add_y+7 + o*(o_config.radius*2 + o_config.border)); 
		cairo_show_text(cr, str);  
		cairo_stroke(cr);
		memset(str,'\0', 80);

		select_next_or(&ors);	
		o++;		
	}

	h = 0;
	select_first_hypersphere(&hyperspheres);
	while(is_active_hypersphere(&hyperspheres)) {

		cairo_set_source_rgb(cr, line, line, line);
		cairo_set_line_join(cr, CAIRO_LINE_JOIN_BEVEL); 
		cairo_set_line_width(cr, 2);

		for(v = 0; v < vectors.dimensions; v++) {

			cairo_move_to(cr, v_config.px_to_add_x + v_config.radius, v_config.px_to_add_y + v*(v_config.radius*2 + v_config.border));
			cairo_line_to(cr, h_config.px_to_add_x - h_config.radius, h_config.px_to_add_y + h*(h_config.radius*2 + h_config.border));
			cairo_stroke(cr);
		}

		o = 0;
		select_first_or(&ors);
		while(is_active_or(&ors)) {

			if(ors.Active == hyperspheres.Active->or_layer) {
				cairo_move_to(cr, o_config.px_to_add_x-o_config.radius, o_config.px_to_add_y + o*(o_config.radius*2 + o_config.border));
				cairo_line_to(cr, h_config.px_to_add_x+h_config.radius, h_config.px_to_add_y + h*(h_config.radius*2 + h_config.border));
				cairo_stroke(cr);
				break;
			}
			select_next_or(&ors);	
			o++;		
		}		

		cairo_set_line_width(cr, 5);
		cairo_set_source_rgb(cr, border_col, border_col, border_col);

		cairo_arc(cr, h_config.px_to_add_x, h_config.px_to_add_y + h*(h_config.radius*2 + h_config.border), h_config.radius, 0, 2 * M_PI);
		cairo_stroke_preserve(cr);
		
		cairo_set_source_rgb(cr, middle_col, middle_col, middle_col);
		cairo_fill(cr);
		
		sprintf(str, "%2.2f", hyperspheres.Active->radius);
		cairo_set_source_rgb(cr, border_col, border_col, border_col);
		cairo_move_to(cr, h_config.px_to_add_x-19, h_config.px_to_add_y+5 + h*(h_config.radius*2 + h_config.border));  
  		cairo_show_text(cr, str);  
  		memset(str,'\0', 80);
		select_next_hypersphere(&hyperspheres);			
		h++;
	}
	
	pthread_mutex_unlock(&lock_mem);

	copy(old_surface, surface);
}


gboolean draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {

	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);

	return FALSE;
}


int main(int argc, char *argv[]) {

	pthread_t rce_thread;

	char *file_name = NULL;
	int c;
	GtkBuilder *builder;
	GtkWidget *window;
	
	opterr = 0;

	while ((c = getopt (argc, argv, "i:")) != -1) {

		switch (c) {
			case 'i':
				file_name = optarg;
				break;
			case '?':
				if (optopt == 'i')
					fprintf (stderr, "Option -%c requires an argument with file name.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return 1;
			default:
				abort ();
		}
	}

	if(file_name != NULL) {

		parser(&vectors, file_name);
	}
	else {

		init_input_vector_list(&vectors, 0, 3.5);
	}



	init_hyperspheres_list(&hyperspheres);
	init_ors_list(&ors);

	if(pthread_create(&rce_thread, NULL, rce_main, NULL)) {

		fprintf(stderr, "Error creating thread\n");
		return 1;
	}


	gtk_init(&argc, &argv);

	builder = gtk_builder_new_from_file("glade/rce1.glade");
	gtk_builder_connect_signals(builder, NULL);

	window = GTK_WIDGET(gtk_builder_get_object (builder, "window1"));
	
	g_object_unref(builder);

	gtk_widget_show_all(window);

	gtk_main();


	dispose_input_vector_list(&vectors);
	dispose_ors_list(&ors);
	dispose_hyperspheres_list(&hyperspheres);

	pthread_cancel(rce_thread);
	pthread_join(rce_thread, NULL);
	pthread_mutex_destroy(&lock_mem);
	pthread_mutex_destroy(&lock_rce);
	pthread_cond_destroy(&cond_rce);

	return 0;
}
