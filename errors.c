/**@<errors.c>::**/
#include <stdio.h>
#include <stdlib.h>
#include <lexer.h>

extern int lineNumber;

void printMismatchError(char const *expected) {
	fprintf(stderr, "token mismatch at line %d: expected '%s' and got '%s'\n",
		lineNumber, expected, lexeme);
	exit(-2);
}

void printDuplicateSymbolError(char const *symbol) {
	fprintf(stderr, "duplicate symbol '%s' at line %d\n", symbol, lineNumber);
	exit(-2);
}

void printUndeclaredSymbolError(char const *symbol) {
	fprintf(stderr, "undeclared symbol '%s' at line %d\n",
		symbol, lineNumber);
	exit(-2);
}

void printInvalidAssignmentError(char const *symbol) {
	fprintf(stderr, "invalid assignment for '%s' at line %d\n",
		symbol, lineNumber);
	exit(-2);
}

void printInvalidCallError(char const *symbol) {
	fprintf(stderr, "invalid call for var '%s' at line %d\n",
		symbol, lineNumber);
	exit(-2);
}
