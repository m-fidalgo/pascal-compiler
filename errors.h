/**@<errors.h>::**/

// Declarando cabeçalho de funções
extern void printMismatchError(char const *expected);
extern void printDuplicateSymbolError(char const *symbol);
extern void printUndeclaredSymbolError(char const *symbol);
extern void printInvalidAssignmentError(char const *symbol);
extern void printInvalidCallError(char const *symbol);
