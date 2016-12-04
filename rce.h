#ifndef RCE_H
#define RCE_H


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


void *rce_main();
// void *rce_main(void *, void *, void *);
void parser(I_VECTORS *, char *);
void next_step();

#endif
