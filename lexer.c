/**@<lexer.c>::**/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <tokens.h>
#include <lexer.h>
#include <keywords.h>

// contador de linhas
int lineNumber = 1;
char lexeme[MAXIDLEN + 1];

void skipspaces (FILE * tape)
{
  int head;
  while (isspace (head = getc (tape))) {
    if (head == '\n') {
      lineNumber++;
    }
  }
  ungetc (head, tape);
}

/** ID = [A-Za-z][A-Za-z0-9]* **/
int isID (FILE * tape)
{
  int i = 0;

  if (isalpha (lexeme[i] = getc (tape))) {
    i++;
  
    // fecho de kleene
    while (isalnum (lexeme[i] = getc (tape))) {
		if (i < MAXIDLEN) i++;
	}
    ungetc (lexeme[i], tape);
    lexeme[i] = 0;
    
    // verificar se é palavra chave
    if ((i = iskeyword(lexeme))) return i;
    return ID;
  }
  ungetc (lexeme[i], tape);
  lexeme[i] = 0;
  return 0;
}

// DEC, OCT ou HEX
int isINT (FILE * tape)
{
  int head;
  int i = 0;

  if (isdigit (lexeme[i] = getc (tape))) {
    if (lexeme[i] == '0') {
      i++;

      // octal: 0[0-7][0-7]*
      if ('0' <= (lexeme[i] = getc (tape)) && lexeme[i] < '8') {
        i++;
        while ('0' <= (lexeme[i] = getc (tape)) && lexeme[i] < '8') i++;
        ungetc (lexeme[i], tape);
        lexeme[i] = 0;
        return OCT;
      }
      // unget do 1
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;

      // hexadecimal: 0[xX][0-9A-Fa-f][0-9A-Fa-f]*
      if (toupper (lexeme[i] = getc (tape)) == 'X') {
        i++;

        if (isxdigit (lexeme[i] = getc (tape))) {
          i++;

          while (isxdigit (lexeme[i] = getc (tape))) i++;
          ungetc (lexeme[i], tape);
          lexeme[i] = 0;
          return HEX;
        }
        //unget do 2
        ungetc (lexeme[i], tape);
        lexeme[i] = 0;
        i--;
      }
      // unget do 1
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
      return DEC;
    }
    i++;
    // se n entra no if, é o i=1
    // fecho de kleene
    while (isdigit (lexeme[i] = getc (tape))) i++;
    ungetc (lexeme[i], tape);
    lexeme[i] = 0;
    return DEC;
  }
  // unget do 0
  ungetc (lexeme[i], tape);
  lexeme[i] = 0;
  return 0;
}

// EE = [eE] ['+''-']? [0-9][0-9]*
int isEE (FILE * tape)
{
  int i = strlen(lexeme);
  int pm;

  if (toupper (lexeme[i] = getc (tape)) == 'E') {
    i++;

    if ((lexeme[i] = getc (tape)) != '+' && lexeme[i] != '-') {
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
      pm = 0;
    } else {
      i++;
      pm = 1;
    }

    if (isdigit (lexeme[i] = getc (tape))) {
      i++;
      while (isdigit (lexeme[i] = getc (tape))) i++;
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
      return 1;
    }

    ungetc (lexeme[i], tape);
    lexeme[i] = 0;
    i--;

    if (pm) {
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
      i--;
    }
  }

  ungetc (lexeme[i], tape);
  lexeme[i] = 0;
  return 0;
}

// NUM = ( DEC | DEC '.'[0-9]* | '.'[0-9][0-9]* ) EE?
int isNUM (FILE * tape)
{
  int dec = isINT (tape);
  int i = strlen(lexeme);
  int token = 0;

  switch (dec) {
  case OCT:
  case HEX:
    return dec;
  }

  if (dec) {
    token = dec;
    if ((lexeme[i] = getc (tape)) == '.') {
      i++;
      token = FLT;
      while (isdigit (lexeme[i] = getc (tape))) i++;
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
    }
    else {
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
    }
  }
  else {
    if ((lexeme[i] = getc (tape)) == '.') {
      i++;
      if (isdigit (lexeme[i] = getc (tape))) {
        i++;
        token = FLT;
        while (isdigit (lexeme[i] = getc (tape))) i++;
        ungetc (lexeme[i], tape);
        lexeme[i] = 0;
      } else {
        ungetc (lexeme[i], tape);
        lexeme[i] = 0;
        i--;
        ungetc (lexeme[i], tape);
        lexeme[i] = 0;
      }
    }
    else {
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
    }
  }
  if (token) {
    if (isEE (tape)) {
      token = FLT;
    }
  }
  return token;
}

// ASGN (:=)
int isASGN(FILE * tape) {
	lexeme[0] = getc(tape);
	if(lexeme[0] == ':') {
		lexeme[1] = getc(tape);
		if(lexeme[1] == '=') {
			lexeme[2] = 0;
			return ASGN;
		}
		ungetc(lexeme[1], tape);
	}
	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}

// GEQ (>=), NEQ (<>), LEQ (<=)
int isRELOP(FILE * tape) {
	lexeme[0] = getc(tape);
	lexeme[2] = 0;
	switch(lexeme[0]) {
			case '>':
				lexeme[1] = getc(tape);
				if(lexeme[1] == '=') return GEQ;
				ungetc(lexeme[1], tape);
				lexeme[1] = 0;
				return '>';
			case '<':
				switch(lexeme[1] = getc(tape)) {
					case '=': return LEQ;
					case '>': return NEQ;
					default:
						ungetc(lexeme[1], tape);
						lexeme[1] = 0;
						return '<';
				}
	}
	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}

int gettoken (FILE * source)
{
  int token;
  skipspaces (source);

  if (token = isID(source))
    return token;
  if (token = isNUM(source))
    return token;
  if (token = isASGN(source))
    return token;
  if (token = isRELOP(source))
    return token;

  token = getc (source);
  return token;
}
