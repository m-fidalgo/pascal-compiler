/**@<symtab.c>::**/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errors.h>
#include <symtab.h>

// Definindo constantes
#define SYMTABSIZE 1024

// Definindo variáveis
SYMTAB symtab[SYMTABSIZE];
int symtab_first;
int symtab_next = 0;
int lexlevel = 1;

// Busca de baixo para cima um símbolo na tabela
int symtab_lookup(char const *symbol) {
	int i;
	for(i = symtab_next - 1; i > -1; i--) {
		if(strcmp(symtab[i].name, symbol) == 0) return i;
	}
	return i;
}

// Insere um símbolo na tabela
int symtab_append(char const *symbol) {
	int sym_pos = symtab_lookup(symbol);
	
	// Se não encontrou o símbolo ou se ele está em outro lexlevel, insere
	if(sym_pos == -1 || symtab[sym_pos].lexlevel != lexlevel) {
		strcpy(symtab[symtab_next].name, symbol);
		symtab[symtab_next].lexlevel = lexlevel;
		return symtab_next++;
	} 
	// Caso contrário, imprime erro
	/**/printDuplicateSymbolError(symbol);/**/
}

// Função que insere o símbolo na tabela e define seu 'objtype'
int symtab_append_and_set_objtype(char const *symbol, int objtype) {
	int position = symtab_append(symbol);
	symtab[position].objtype = objtype;
	return position;
}

// Função que define o 'type' de um símbolo na tabela
void symtab_set_type(int position, int type) {
	symtab[position].type = type;
}

// Função que define o 'type' para a lista recente de símbolos na tabela
void symtab_set_type_for_range(int type) {
	for(int i = symtab_first; i < symtab_next; i++)
		symtab_set_type(i, type);
}

// Imprime o conteúdo da tabela
void symtab_list() {
	for(int i = 0; i < symtab_next; i++) {
		printf("[%-2d] name: %-15s objtype: %-5d type: %-5d lexlevel: %-5d\n",
				i, symtab[i].name, symtab[i].objtype, symtab[i].type,
				symtab[i].lexlevel);
	}
}

// Retorna uma entrada da tabela, dada uma posição
SYMTAB symtab_get(int pos) {
	return symtab[pos];
}
