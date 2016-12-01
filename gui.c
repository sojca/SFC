#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gui.h"
#include "vector_editor.h"

#include "hypersphere.h"
#include "input.h"
#include "or.h"
#include "rce.h"

static cairo_surface_t *surface;

gboolean started = TRUE;

I_VECTORS vectors;
HYPERSPHERES hyperspheres;
ORS ors;

gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	return FALSE;
}

void destroy_signal(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

static void clear(GtkWidget *widget, gpointer data)
{
	cairo_t *cr = cairo_create(surface);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
	cairo_destroy(cr);
	gdk_window_invalidate_rect(gtk_widget_get_window(data), NULL, FALSE);
}

gboolean next_rce_step(GtkWidget *widget, GdkEventButton *event, gpointer *data)
{
	return TRUE;
}

gboolean clear_rce_drawing(GtkWidget *widget, GdkEventButton *event, gpointer *data)
{
	return TRUE;
}

gboolean stop_rce_step(GtkWidget *widget, GdkEventButton *event, gpointer *data)
{
	return TRUE;
}


 gboolean start_rce_step(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	
	started = TRUE;
	
	dispose_hyperspheres_list(&hyperspheres);
	dispose_ors_list(&ors);

	rce_main(&vectors, &hyperspheres, &ors);	
	// draw_release();	
	GtkWidget *toplevel;

	toplevel = gtk_widget_get_toplevel(widget);
	// GtkBuilder *builder;
	// GtkWidget *draw;
	
	// builder = gtk_builder_new_from_file("glade/rce.glade");
	// gtk_builder_connect_signals(builder, NULL);

	// draw = GTK_WIDGET(gtk_builder_get_object (builder, "drawingarea1"));
	gtk_widget_queue_draw (toplevel);

	// time_handler(draw);

	return TRUE;
}

gboolean open_vector_editor(GtkWidget *widget, GdkEventButton *event, gpointer *data)
{
	int result = create_vector_editor();

	printf("ok %i\n", result == GTK_RESPONSE_OK);
	printf("cancel %i\n", result == GTK_RESPONSE_CANCEL);
	
	return TRUE;
}


 gboolean draw_config(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	GtkAllocation allocation;

	gtk_widget_get_allocation(widget, &allocation);

	surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
			CAIRO_CONTENT_COLOR, allocation.width, allocation.height);
		
	clear(widget, widget);
	g_print("config %d,%d\n", allocation.width, allocation.height);
	return TRUE;
}

 gboolean draw_event(GtkWidget *widget, cairo_t *cr, gpointer data)
{
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);

	if(started == FALSE) return FALSE;

	started = FALSE;
	int v, h, o;

	double middle = 0.96;
	double border = 0.2;
	double line = 0.2;

	for(v = 1; v <= vectors.dimensions; v++){

		cairo_set_line_width(cr, 4);
		cairo_set_source_rgb(cr, border, border, border);

		cairo_arc(cr, 50, v*50, 10, 0, 2 * M_PI);
		cairo_stroke_preserve(cr);
		
		cairo_set_source_rgb(cr, middle, middle, middle);
		cairo_fill(cr);

	}

	o = 1;
	select_first_or(&ors);
	while(is_active_or(&ors)){
		printf("%i\n", ors.Active->class);

		cairo_set_line_width(cr, 4);
		cairo_set_source_rgb(cr, border, border, border);

		cairo_arc(cr, 450, o*70, 20, 0, 2 * M_PI);
		cairo_stroke_preserve(cr);
		
		cairo_set_source_rgb(cr, middle, middle, middle);
		cairo_fill(cr);

		select_next_or(&ors);	
		o++;		
	}

	h = 1;
	select_first_hypersphere(&hyperspheres);
	while(is_active_hypersphere(&hyperspheres)){
		// printf("%i\n", hyperspheres.Active->class);
		cairo_set_source_rgb(cr, line, line, line);
		cairo_set_line_join(cr, CAIRO_LINE_JOIN_BEVEL); 
		cairo_set_line_width(cr, 4);

		for(v = 1; v <= vectors.dimensions; v++){
			cairo_move_to(cr, 61, v*50);
			cairo_line_to(cr, 220, h*80);
		}
		cairo_stroke(cr);

		o = 1;

		select_first_or(&ors);
		while(is_active_or(&ors)){
			printf("%i\n", ors.Active->class);

			if(ors.Active == hyperspheres.Active->or_layer){
				cairo_move_to(cr, 430, o*70);
				cairo_line_to(cr, 220, h*80);
				cairo_stroke(cr);
				break;
			}
			select_next_or(&ors);	
			o++;		
		}		

		cairo_set_source_rgb(cr, border, border, border);
		cairo_set_line_width(cr, 9);

		cairo_arc(cr, 250, 0 + h*80, 30, 0, 2 * M_PI);
		cairo_stroke_preserve(cr);
		
		cairo_set_source_rgb(cr, middle, middle, middle);
		cairo_fill(cr);

		select_next_hypersphere(&hyperspheres);			
		h++;
	}


	// cairo_set_source_rgb(cr, 0, 1, 1);

	// cairo_move_to(cr, 12, 10);

	// cairo_line_to(cr, 100, 100);
	// cairo_stroke(cr);




	g_print("draw\n");
	return FALSE;
}

int main(int argc, char *argv[])
{
	// GtkWidget *draw;
	// /* inicializace GTK+ */
	gtk_init(&argc, &argv);

	// int aflag = 0;
	// int bflag = 0;
	char *file_name = NULL;
	// int index;
	int c;

	opterr = 0;
	while ((c = getopt (argc, argv, "i:")) != -1){
		switch (c) {
			// case 'a':
			// 	aflag = 1;
			// 	break;
			// case 'b':
			// 	bflag = 1;
			// 	break;
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
	// printf ("aflag = %d, bflag = %d, cvalue = %s\n", aflag, bflag, cvalue);

	// for (index = optind; index < argc; index++)
		// printf ("Non-option argument %s\n", argv[index]);

	if(file_name != NULL)
		parser(&vectors, file_name);
	else 
		init_input_vector_list(&vectors, 0, 3.5);

	init_hyperspheres_list(&hyperspheres);
	init_ors_list(&ors);


	GtkBuilder *builder;
	GtkWidget *window;
	// GtkWidget *draw;
	
	builder = gtk_builder_new_from_file("glade/rce.glade");
	gtk_builder_connect_signals(builder, NULL);

	window = GTK_WIDGET(gtk_builder_get_object (builder, "window1"));
	// draw = GTK_WIDGET(gtk_builder_get_object (builder, "drawingarea1"));
	
	g_object_unref(builder);
	// gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK );
	// g_signal_connect(G_OBJECT(window), "button-press-event", G_CALLBACK(start_rce_step), NULL);
	
	// g_signal_connect(G_OBJECT(draw), "button-release-event", G_CALLBACK(draw_release), NULL);
	// gtk_widget_add_events(draw, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK 	);
	
	// g_timeout_add(100, (GSourceFunc) time_handler, (gpointer) window);
	
// ######################### BUILDER STUFF END ##########################
	gtk_widget_show_all(window);
	gtk_main();

	dispose_input_vector_list(&vectors);
	dispose_ors_list(&ors);
	dispose_hyperspheres_list(&hyperspheres);

	return 0;
}
