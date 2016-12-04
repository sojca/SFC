CDEBUGFLAGS =	-O2 -Wall -g -DGTK_DISABLE_DEPRECATED=1 -DGDK_DISABLE_DEPRECATED=1 -DG_DISABLE_DEPRECATED=1
CFLAGS 		=	${CDEBUGFLAGS} `pkg-config --cflags gtk+-3.0`
LDLIBS 		=	`pkg-config --libs gtk+-3.0`
LDFLAGS 	=	-g -std=gnu99 -Wall -Wextra -Werror -pedantic

OBJ 		=	or.o input.o hypersphere.o rce.o vector_editor.o gui.o
PROG		=	rce-demonstrator

all: ${OBJ}
	$(CC) ${LDFLAGS} ${CFLAGS} ${OBJ}  -rdynamic -o ${PROG} -lm  $(LDLIBS)


vector_editor: vector_editor.o input.h
	$(CC) $(LDFLAGS) -c $(LDLIBS)

gui: gui.o
	$(CC) $(LDFLAGS) -c $(LDLIBS)

or.o: or.c
	$(CC) $(LDFLAGS) or.c -c

input.o: input.c
	$(CC) $(LDFLAGS) input.c -c

hypersphere.o: hypersphere.c input.h or.h
	$(CC) $(LDFLAGS) hypersphere.c -c

rce.o: rce.c 
	$(CC) $(LDFLAGS) rce.c -c

clean: ${OBJ}
	rm -rf ${OBJ} $(PROG) *zip

zip: ${OBJ}
	zip -r xsojca00.zip . -x *.git* $(PROG) *o *~ *zip
