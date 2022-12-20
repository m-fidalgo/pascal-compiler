/**@<keywords.c>::**/
#include <string.h>
#include <keywords.h>

// Palavras reservadas do Pascal são consideradas keywords
char *keyword[] = {
	"begin",
	"program",
	"procedure",
	"function",
	"var",
	"integer",
	"real",
	"double",
	"boolean",
	"not",
	"or",
	"and",
	"div",
	"mod",
	"true",
	"false",
	"if",
	"then",
	"else",
	"while",
	"do",
	"repeat",
	"until",
	"end",
};

// Função que verifica se uma dada palavra é uma keyword
int iskeyword(char const *name) {
	for(int i = BEGIN; i <= END; i++) {
		if(strcmp(keyword[i - BEGIN], name) == 0) return i;
	}
	return 0;
}
