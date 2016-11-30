#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "hypersphere.h"
#include "input.h"
#include "or.h"
// #include "gui.h"


void parser(I_VECTORS *list_vect, double *radius) 
{

	FILE *fp;
	fp = fopen("vector.i", "r");

	char c[2];
	memset(c, 0, 2);

	char buff[128];
	memset(buff, 0, 128);

	int class;
	int *vect;
	vect = NULL;
	int i = 0;
	int max = 0;
	int state = 0;

	while((c[0] = fgetc(fp)) != EOF){
		switch(state) {
			case(0):
				if(c[0] == 'r')
					state = 1;
				if(c[0] == 'd')
					state = 5;
				else if(c[0] == '(')
					state = 10;
				else if(isspace(c[0]))
					continue;

				continue;
				break;
			
			case(1):
				if(isdigit(c[0])){
					state = 2;
					strncat(buff, c, 1);
				}
				else if(c[0] == '.'){
					state = 3;
					strncat(buff, c, 1);
				}
				else if(isspace(c[0]))
					continue;
				continue;
				break;
			
			case(2):
				if(isdigit(c[0])){
					strncat(buff, c, 1);
				}
				else if(c[0] == '.'){
					state = 3;
					strncat(buff, c, 1);
				}
				else {
					*radius = strtof(buff, NULL);
					memset(buff, 0, 128);

					state = 0;
				}
				continue;
				break;

			case(3):
				if(isdigit(c[0]))
					strncat(buff, c, 1);
				else {
					*radius = strtof(buff, NULL);
					memset(buff, 0, 128);

					state = 0;
				}
				continue;
				break;
			case(5):
				if(isdigit(c[0])){
					state = 6;
					strncat(buff, c, 1);
				}
				else if(isspace(c[0]))
					continue;
				continue;
				break;
			case(6):
				if(isdigit(c[0])){
					strncat(buff, c, 1);
					continue;
				}
				sscanf(buff, "%i", &max);
				init_input_vector_list(list_vect, max, 0);
				memset(buff, 0, 128);
				state = 0;
				continue;
				break;
			case(10):
				if(isdigit(c[0])){
					strncat(buff, c, 1);
					state = 11;
				} 
				else if(isspace(c[0]))
					continue;
				continue;
			
			case(11):
				if(isdigit(c[0])){
					strncat(buff, c, 1);
					continue;
				} 
				else if(c[0] == ','){
					vect = (int *)realloc(vect, sizeof(int)*(i+1));
					sscanf(buff, "%i", &vect[i]);
					memset(buff, 0, 128);
					i++;
					state = 10;
				} 
				else if(c[0] == ')'){
					vect = (int *)realloc(vect, sizeof(int)*(i+1));
					sscanf(buff, "%i", &vect[i]);
					memset(buff, 0, 128);
					i++;
					state = 13;
				}
				else if(isspace(c[0])){
					vect = (int *)realloc(vect, sizeof(int)*(i+1));
					sscanf(buff, "%i", &vect[i]);
					memset(buff, 0, 128);
					i++;
					state = 12;
				}
				continue;
				break;
			case(12):
				if(c[0] == ','){
					state = 10;
				} 
				else if(c[0] == ')'){
					state = 13;
				}	
				break;
			
			case(13):
				if(isdigit(c[0])){
					strncat(buff, c, 1);
					state = 14;
					continue;
				} 
				break;					
			case(14):
				if(isdigit(c[0])){
					strncat(buff, c, 1);
					state = 14;
					continue;
				}
				sscanf(buff, "%d", &class);
				memset(buff, 0, 128);

				insert_last_input_vector(list_vect, vect, class);

				free(vect);
				vect = NULL;
				i = 0;
				
				state = 0;
				break;
			default:
				break;
		}



	} 
	// while((c[0] = fgetc(fp)) != EOF);


	// select_first_input_vector(list_vect);
	// while(is_active_input_vector(list_vect)){
	// 	for(i = 0; i < max; i++)
	// 		printf("%2i ", list_vect->Active->i[i]);
	// 	printf("%3i\n", list_vect->Active->class);
	// 	select_next_input_vector(list_vect);
	// }

	free(vect);
	fclose(fp);

}


// int main(int argc, char *argv[])
// {

// 	// start(argc, argv);

// 	bool modif = false;
// 	bool hit = false;
	
// 	double radius = 0;
// 	I_VECTORS i_vector; 
// 	HYPERSPHERES h;
// 	ORS o;
	
// 	init_hyperspheres_list(&h);
// 	init_ors_list(&o);

// 	parser(&i_vector, &radius);

	
// 	do {
// 		modif = false;
// 		select_first_input_vector(&i_vector);
// 		while(is_active_input_vector(&i_vector)){
// 			// printf("%i\n", i_vector.Active->i[0]);
// 			hit = false;
// 			select_first_hypersphere(&h);
// 			while(is_active_hypersphere(&h)){
// 				double distance = 0;

// 				for(unsigned i = 0; i < i_vector.lenght; i++){
// 					distance += pow(i_vector.Active->i[i] - h.Active->i[i], 2);
// 				}
// 				distance = sqrt(distance);

// 				if (distance <= h.Active->radius){
// 					if(h.Active->class == i_vector.Active->class)
// 						hit = true;
// 					else{
// 						h.Active->radius = distance / 2.0;	
// 						modif = true;
// 					}
// 				}

// 				select_next_hypersphere(&h);			
// 			}
// 			if(!hit){
// 				insert_last_hypersphere(&h, radius, i_vector.Active->class, i_vector.Active->i, i_vector.lenght);
// 				select_last_hypersphere(&h);	

// 				modif = true;
// 				if(is_exist_or_with_class(&o, h.Active->class)){
// 					h.Active->or_layer = get_or_with_class(&o, h.Active->class);
// 				}
// 				else{
// 					insert_last_or(&o, h.Active->class);
// 					select_last_or(&o);

// 					h.Active->or_layer = o.Active;
// 				}
// 			}


// 			select_next_input_vector(&i_vector);
// 		}
// 	} while(modif);


// 	select_first_hypersphere(&h);
// 	while(is_active_hypersphere(&h)){
// 		printf("%f\n", h.Active->radius);
// 		for(unsigned i = 0; i < i_vector.lenght; i++){
// 			printf("%3i ", h.Active->i[i]);
// 		}
// 		printf("\n--\n");
// 		select_next_hypersphere(&h);			
// 	}


// 	select_first_or(&o);
// 	while(is_active_or(&o)){
// 		printf("%i\n", o.Active->class);
// 		printf("--\n");
// 		select_next_or(&o);			
// 	}

// 	start(argc, argv, &i_vector, &h, &o);


// 	dispose_input_vector_list(&i_vector);
// 	dispose_ors_list(&o);
// 	dispose_hyperspheres_list(&h);


// 	return 0;
// }
