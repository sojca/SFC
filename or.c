#include <stdio.h>
#include <stdlib.h>

#include "or.h"


void init_ors_list (ORS *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
** ---------------------------------------------------------------------
** ukazateľ na prvý a aktuálny prvok sa vynulujú 
**/

	L->Active = NULL;
	L->First = NULL;	
	L->Last = NULL;	
}

void dispose_ors_list (ORS *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
** ---------------------------------------------------------------------
** Ptr,DelPtr - pomocné ukazatele
** do Ptr priradíme prvý, resp. nasledujúci prvok
** do DelPtr priradzujeme Ptr, tak aby sme nestratili Ptr pri uvoľnení DelPtr
** nakoniec vynulujeme ukazatele First a Act
**/

	OR ptr, delptr;
	ptr = L->First;
	
	while(ptr != NULL)
	{
		delptr = ptr;
		ptr = ptr->next;
		free(delptr);
	}	

	L->Active = NULL;
	L->First = NULL;
	L->Last = NULL;
}

void insert_first_or (ORS *L, int class) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
** ---------------------------------------------------------------------
** do pomocného ukazateľa Ptr sme alokovali miesto o veľkosti štruktúry tElem,
** vložíme data do štruktúry, do ukazateľa na naseldujúci prvok ptr vložíme,
** ukazateľ na prvý prvok a následne ukazateľ Ptr spravíme ako nový prvý prvok
** ak nie je miesto/chyba malloc, tak voláme Error()
**/

	OR ptr = malloc(sizeof(struct or));
	
	if(ptr == NULL)
		fprintf(stderr, "Malloc err\n");
		// Error();

	ptr->class = class;
	ptr->next = L->First;
	ptr->prev = NULL;

	if(L->First == NULL)
		L->Last = ptr;
	else
		L->First->prev = ptr;
	L->First = ptr;
}

void insert_last_or (ORS *L, int class) {

	OR ptr = malloc(sizeof(struct or));
	
	if(ptr == NULL)
		fprintf(stderr, "Malloc err\n");
		// Error();

	ptr->class = class;
	ptr->next = NULL;
	ptr->prev = L->Last;

	if(L->Last == NULL)
		L->First = ptr;
	else
		L->Last->next = ptr;
	L->Last = ptr;
}

void select_first_or(ORS *L) {

	L->Active = L->First;
}

void select_last_or(ORS *L) {

	L->Active = L->Last;
}

void select_next_or(ORS *L){
	
	if(L->Active != NULL)
		L->Active = L->Active->next;
}

int is_active_or(ORS *L){
	
	return (L->Active != NULL)?TRUE:FALSE;	
}

int is_exist_or_with_class(ORS *L, int class){
	
	OR ptr = L->First;
	while(ptr != NULL){

		if(class == ptr->class)
			return TRUE;
		ptr = ptr->next;
	}
	return FALSE;	
}

OR get_or_with_class(ORS *L, int class){
	
	OR ptr = L->First;
	while(ptr != NULL){

		if(class == ptr->class)
			return ptr;
		ptr = ptr->next;
	}
	return NULL;	
}
