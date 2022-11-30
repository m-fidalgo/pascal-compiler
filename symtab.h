/**@<symtab.h>::**/
#include <lexer.h>

typedef struct _symtab_ {
	char name[MAXIDLEN+1];
	int objtype; // 0 proc, 1 func, 2 var
	int type; // 0 vazio, 1 int, 2 real, 3 double, 4 bool
	int lexlevel; // número de reentrâncias
} SYMTAB;

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
