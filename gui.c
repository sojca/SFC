#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include <math.h>

#include "gui.h"
#include "vector_editor.h"

#include "hypersphere.h"
#include "input.h"
#include "or.h"
#include "rce.h"

static cairo_surface_t *surface;

I_VECTORS vectors;

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

gboolean draw_release(GtkWidget *widget, GdkEventButton *event, HYPERSPHERES *data)
{
	if (surface == NULL) return FALSE;

	cairo_t *cr;
	cr = cairo_create(surface);

	cairo_move_to(cr, 12, 10);

	cairo_line_to(cr, 100, 100);
	cairo_stroke(cr);

	select_first_hypersphere(data);
	while(is_active_hypersphere(data)){
		printf("%f\n", data->Active->radius);
	// 	// for(unsigned i = 0; i < i_vector.lenght; i++){
	// 		// printf("%3i ", h.Active->i[i]);
	// 	// }
		printf("\n--\n");
		select_next_hypersphere(data);			
	}	

	// if((event->state & GDK_SHIFT_MASK) == GDK_SHIFT_MASK)
		// draw_resource(cr, event->x, event->x);
	// else
	// draw_resource(cr, event->x, event->y, event->state);
	
	cairo_destroy(cr);

	/* invalidace obdelniku cary v okne, vynuceni expose */
	gdk_window_invalidate_rect(gtk_widget_get_window(widget), NULL, FALSE);

	return TRUE;
}

gboolean next_rce_step(GtkWidget *widget, GdkEventButton *event, gpointer *data)
{
	return TRUE;
}

gboolean clear_rce_drawing(GtkWidget *widget, GdkEventButton *event, gpointer *data)
{
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


	g_print("draw\n");
	return FALSE;
}

int main(int argc, char *argv[])
{
	// GtkWidget *draw;
	// /* inicializace GTK+ */
	gtk_init(&argc, &argv);


	// GtkWidget *b;
	// GtkAdjustment *adjustment;

	// adjustment = gtk_adjustment_new (2.50, 0.0, 5.0, 0.01, 0.1, 0.0);

	// b = gtk_spin_button_new (adjustment, 0.01, 2);



	// window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// g_signal_connect(G_OBJECT(window), "destroy",
	// 	G_CALLBACK(destroy_signal), NULL);
	// g_signal_connect(G_OBJECT(window), "delete_event",
	// 	G_CALLBACK(delete_event), NULL);

	// /* hlavni kontejner */
	// /* gtk_vbox deprecated */
	// vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	// // hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// gtk_container_add(GTK_CONTAINER(window), vbox);

	// /* kreslici plocha */
	// draw = gtk_drawing_area_new();

	// gtk_widget_set_size_request(draw, 300, 300);
	// gtk_box_pack_start(GTK_BOX(vbox), draw, TRUE, TRUE, 0);
	// gtk_box_pack_start(GTK_BOX(vbox), b, TRUE, TRUE, 0);

	// unsigned i = 0;
	// select_first_hypersphere(h);
	// while(is_active_hypersphere(h)){
	// 	printf("%f\n", (h->Active)->radius);
	// 	for(i = 0; i < i_vector->lenght; i++){
	// 		printf("%3i ", (h->Active)->i[i]);
	// 	}
	// 	printf("\n--\n");
	// 	select_next_hypersphere(h);			
	// }	



	// g_signal_connect(G_OBJECT(draw), "draw",
	// 	G_CALLBACK(draw_event), NULL);
	// g_signal_connect(G_OBJECT(draw), "configure-event",
	// 	G_CALLBACK(draw_config), NULL);
	// g_signal_connect(G_OBJECT(draw), "button-release-event",
		// G_CALLBACK(draw_release), (HYPERSPHERES*) h);
		// G_CALLBACK(draw_release), NULL);

	// /* registrace vstupnich udalosti pro okno, masky viz XSelectInput() */
	// gtk_widget_add_events(draw, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK );

	// // button = gtk_button_new_with_label("Clear");
	// // gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	// // g_signal_connect(G_OBJECT(button), "clicked",
	// // 	G_CALLBACK(clear), draw);

	
	// // gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
	
	// /* zobrazit okna */   
	// gtk_widget_show_all(window);

// ######################### BUILDER STUFF START ########################
	

	// GdkPixmap *pixmap;
 //    GtkWidget *image;
 //    GtkWidget *window;
 //    cairo_t *cr;

 //    gtk_init(&argc, &argv);

 //    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 //    g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
	
	// image = gtk_image_new();

 //    gtk_widget_show_all(window);

 //    pixmap = gdk_pixmap_new(window->window, 100, 100, -1);
 //    cr = gdk_cairo_create(pixmap);
 //    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);    
 //    cairo_rectangle(cr, 10, 10, 80, 80);
 //    cairo_fill(cr);
 //    cairo_destroy(cr);

 //    cr = NULL;



	// gtk_image_set_from_pixmap(GTK_IMAGE(image), pixmap, NULL);
 //    // image = gtk_image_new_from_pixmap(pixmap, NULL);

 //    gtk_container_add(GTK_CONTAINER(window), image);


 //    gtk_widget_show(image);



 //    gtk_main();
	init_input_vector_list(&vectors, 0, 3.5);

	GtkBuilder *builder;
	GtkWidget *window;
	
	builder = gtk_builder_new_from_file("glade/rce.glade");
	gtk_builder_connect_signals(builder, NULL);

	window = GTK_WIDGET(gtk_builder_get_object (builder, "window1"));
	
	g_object_unref(builder);

	
// ######################### BUILDER STUFF END ##########################
	gtk_widget_show_all(window);

	gtk_main();

	dispose_input_vector_list(&vectors);

	return 0;
}
