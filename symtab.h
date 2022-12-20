/**@<symtab.h>::**/
#include <lexer.h>

// Definição da tabela de símbolos
typedef struct _symtab_ {
	// Nome do símbolo
	char name[MAXIDLEN+1];
	// Tipo de objeto: 0 para procedure, 1 para function e 2 para var
	int objtype;
	// Tipo do dado relacionado: 0 para vazio, 1 para int, 2 real, 3 double e 4 bool
	int type;
	// Nível léxico do símbolo: número de reentrâncias
	int lexlevel;
} SYMTAB;

// Definindo cabeçalho de funções
extern int lexlevel;
extern SYMTAB symtab[];
extern int symtab_first;
extern int symtab_next;
extern int symtab_lookup(char const *symbol);
extern int symtab_append(char const *symbol);
extern int symtab_append_and_set_objtype(char const *symbol, int objtype);
extern void symtab_set_type(int position, int type);
extern void symtab_set_type_for_range(int type);
extern void symtab_list();
extern SYMTAB symtab_get(int pos);
