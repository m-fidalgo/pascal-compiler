/**@<symtab.c>::**/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <symtab.h>

#define SYMTABSIZE 1024

SYMTAB symtab[SYMTABSIZE];
int symtab_first;
int symtab_next = 0;
int lexlevel = 1;

// busca de baixo para cima
int symtab_lookup(char const *symbol) {
	int i;
	for(i = symtab_next - 1; i > -1; i--) {
		if(strcmp(symtab[i].name, symbol) == 0) return i;
	}
	return i;
}

int symtab_append(char const *symbol) {
	int sym_pos = symtab_lookup(symbol);
	if(sym_pos == -1 || symtab[sym_pos].lexlevel != lexlevel) {
		strcpy(symtab[symtab_next].name, symbol);
		symtab[symtab_next].lexlevel = lexlevel;
		return symtab_next++;
	} 
	fprintf(stderr, "'%s' is a duplicate symbol\n", symbol);
	exit(-2);
}

int symtab_append_and_set_objtype(char const *symbol, int objtype) {
	int position = symtab_append(symbol);
	symtab[position].objtype = objtype;
	return position;
}

void symtab_set_type(int position, int type) {
	symtab[position].type = type;
}

void symtab_set_type_for_range(int type) {
	for(int i = symtab_first; i < symtab_next; i++)
		symtab_set_type(i, type);
}

void symtab_list() {
	for(int i = 0; i < symtab_next; i++) {
		printf("[%-2d] name: %-15s objtype: %-5d type: %-5d lexlevel: %-5d\n",
				i, symtab[i].name, symtab[i].objtype, symtab[i].type,
				symtab[i].lexlevel);
	}
}


