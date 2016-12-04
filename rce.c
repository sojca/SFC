#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <pthread.h>

#include "hypersphere.h"
#include "input.h"
#include "or.h"
#include "rce.h"

extern pthread_mutex_t lock;
extern pthread_cond_t cond;


extern I_VECTORS vectors;
extern HYPERSPHERES hyperspheres;
extern ORS ors;


void parser(I_VECTORS *list_vect, char *file_name) 
{

	double radius = 0;
	FILE *fp;
	fp = fopen(file_name, "r");

	char c[2];
	memset(c, 0, 2);

	char buff[128];
	memset(buff, 0, 128);

	int class;
	int *vect;
	vect = NULL;

	int i = 0;
	int dimensions = 0;
	
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
					radius = strtof(buff, NULL);
					memset(buff, 0, 128);

					state = 0;
				}
				continue;
				break;

			case(3):
				if(isdigit(c[0]))
					strncat(buff, c, 1);
				else {
					radius = strtof(buff, NULL);
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
				sscanf(buff, "%i", &dimensions);
				init_input_vector_list(list_vect, dimensions, radius);
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
	set_radius(list_vect, radius);

	free(vect);
	fclose(fp);
}

void next_step(){
	printf("rce next_step\n");
	// pthread_mutex_unlock(&lock);
	pthread_cond_signal(&cond);
}


void *rce_main()
// void *rce_main(void *i_vector, void *h, void *o)
// void *rce_main(I_VECTORS *i_vector, HYPERSPHERES *h, ORS *o)
{

	int i;
	bool modif = false;
	bool hit = false;
	
	printf("waiting to release ...\n");
	// pthread_mutex_lock(&lock);
	// pthread_mutex_lock(&lock);
pthread_cond_wait(&cond, &lock);
	printf("RELEASED!\n");

	do {
		modif = false;

		select_first_input_vector(&vectors);
		while(is_active_input_vector(&vectors)){
			hit = false;
			

			select_first_hypersphere(&hyperspheres);
			while(is_active_hypersphere(&hyperspheres)){
				double distance = 0;

				for(i = 0; i < (&vectors)->dimensions; i++){
					distance += pow((&vectors)->Active->i[i] - (&hyperspheres)->Active->i[i], 2);
				}
				distance = sqrt(distance);

				if (distance <= (&hyperspheres)->Active->radius){
					if((&hyperspheres)->Active->class == (&vectors)->Active->class){
						hit = true;
					}
					else{
						(&hyperspheres)->Active->radius = distance / 2.0;	
						modif = true;
					}
				}

				select_next_hypersphere(&hyperspheres);			
			}

			if(!hit){
				insert_last_hypersphere((&hyperspheres), (&vectors)->radius, (&vectors)->Active->class, (&vectors)->Active->i, (&vectors)->dimensions);
				select_last_hypersphere(&hyperspheres);	

				modif = true;
				if(is_exist_or_with_class((&ors), (&hyperspheres)->Active->class)){
					(&hyperspheres)->Active->or_layer = get_or_with_class((&ors), (&hyperspheres)->Active->class);
				}
				else{
					insert_last_or((&ors), (&hyperspheres)->Active->class);
					select_last_or(&ors);

					(&hyperspheres)->Active->or_layer = (&ors)->Active;
				}
			}
			
			// pthread_mutex_lock(&lock);
			
			pthread_cond_wait(&cond, &lock);


			select_next_input_vector(&vectors);
		}

	} while(modif);


	return NULL;
}
