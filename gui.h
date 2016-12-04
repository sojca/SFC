#ifndef GUI_H
#define GUI_H

#include "hypersphere.h"
#include "input.h"
#include "or.h"


typedef struct config {
	int count;
	int height;
	int px_to_add_x;
	int px_to_add_y;
	int radius;
	int border;
} CONFIG_ITEM;

void config_items();
void draw_network();
// int start(int, char**, I_VECTORS* , HYPERSPHERES*, ORS*);

#endif
