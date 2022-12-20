/**@<keywords.h>::**/
#pragma once

// Definindo as constantes que representam as keywords
enum {
	BEGIN = 8192,
	PROGRAM,
	PROCEDURE,
	FUNCTION,
	VAR,
	INTEGER,
	REAL,
	DOUBLE,
	BOOLEAN,
	NOT,
	OR,
	AND,
	DIV,
	MOD,
	TRUE,
	FALSE,
	IF,
	THEN,
	ELSE,
	WHILE,
	DO,
	REPEAT,
	UNTIL,
	END,
};

// Definindo cabeçalho de funções
extern char *keyword[];
extern int iskeyword(char const *);
