#ifndef RCE_H
#define RCE_H


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#define SET_INPUT 0
#define CLEARED 1
#define START 2
#define STARTED 3

#define NEW_HYPERSPHERE 10
#define NEW_HYPERSPHERE_WITH_OR 11
#define HIT 12
#define CHANGED_RADIUS 13
#define NOT_END 41
#define END 42


void *rce_main();
int parser(I_VECTORS *, char *);
void next_step();

#endif
