/**@<errors.c>::**/
#include <stdio.h>
#include <stdlib.h>
#include <lexer.h>

// Declarando variáveis
extern int lineNumber;

// Erro de token mismatch
void printMismatchError(char const *expected) {
	fprintf(stderr, "token mismatch at line %d: expected '%s' and got '%s'\n",
		lineNumber, expected, lexeme);
	exit(-2);
}

// Erro de símbolo já existente
void printDuplicateSymbolError(char const *symbol) {
	fprintf(stderr, "duplicate symbol '%s' at line %d\n", symbol, lineNumber);
	exit(-2);
}

// Erro de símbolo inexistente
void printUndeclaredSymbolError(char const *symbol) {
	fprintf(stderr, "undeclared symbol '%s' at line %d\n",
		symbol, lineNumber);
	exit(-2);
}

// Erro: houve tentativa de assign num tipo incompatível de símbolo
void printInvalidAssignmentError(char const *symbol) {
	fprintf(stderr, "invalid assignment for '%s' at line %d\n",
		symbol, lineNumber);
	exit(-2);
}

// Erro: houve tentativa de invocar um tipo incompatível de símbolo
void printInvalidCallError(char const *symbol) {
	fprintf(stderr, "invalid call for var '%s' at line %d\n",
		symbol, lineNumber);
	exit(-2);
}
