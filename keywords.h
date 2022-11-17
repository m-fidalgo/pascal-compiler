/**@<keywords.h>::**/
#pragma once

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

extern int iskeyword(char const *);
