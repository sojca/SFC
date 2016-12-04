#ifndef OR_H
#define OR_H


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct or
{
	int class;

	struct or *next;
	struct or *prev;
} *OR;


typedef struct { 
    OR Active;
    OR First;
    OR Last;
} ORS;


void 	init_ors_list (ORS *);
void 	dispose_ors_list (ORS *);
void 	insert_first_or (ORS *, int);
void 	insert_last_or (ORS *, int);
void 	select_first_or (ORS *);
void 	select_last_or (ORS *);
void 	select_next_or(ORS *);
int 	is_active_or(ORS *);
int 	is_exist_or_with_class(ORS *, int);
OR 		get_or_with_class(ORS *, int);
int 	size_of_ors_list(ORS *);

#endif
