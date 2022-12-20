/**@<lexer.c>::**/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <tokens.h>
#include <lexer.h>
#include <keywords.h>

// Contador de linhas: incrementado ao encontrar '\n'
int lineNumber = 1;
// Variável usada para ler os caracteres na fita de entrada
char lexeme[MAXIDLEN + 1];

// Função que ignora espaços e comentários
void skipspaces (FILE * tape)
{
  int head;
_skipspaces:
  // ignorando espaços
  while (isspace (head = getc (tape))) {
    if (head == '\n') {
      lineNumber++;
    }
  }

  // ignorando comentários
  if(head == '{') {
	  while((head = getc(tape)) != '}' && head != EOF) {
		  if (head == '\n') {
			lineNumber++;
		  }
	  }
    // ao final do comentário, deve-se checar se há mais espaços ou comentários
	  goto _skipspaces;
  }
  ungetc (head, tape);
}

// ID = [A-Za-z][A-Za-z0-9]* 
int isID (FILE * tape)
{
  int i = 0;

  // Se o caracter de início é alfanumérico, sabe-se que é um ID
  if (isalpha (lexeme[i] = getc (tape))) {
    i++;
  
    // Fecho de kleene: continua-se lendo os caracteres enquanto forem alfanuméricos
    while (isalnum (lexeme[i] = getc (tape))) {
      if (i < MAXIDLEN) i++;
    }

    // Retornar o caracter que não é alfanumérico
    ungetc (lexeme[i], tape);
    lexeme[i] = 0;
    
    // Verifica-se se é palavra chave, retornando-a em caso positivo e ID caso contrário
    if ((i = iskeyword(lexeme))) return i;
    return ID;
  }

  // Retornar o caracter obtido (não era uma letra, portanto não é um ID)
  ungetc (lexeme[i], tape);
  lexeme[i] = 0;
  return 0;
}

// INT: DEC, OCT ou HEX
int isINT (FILE * tape)
{
  int head;
  int i = 0;

  // Se o primeiro caracter é um dígito, continua-se a leitura para identificar seu tipo
  if (isdigit (lexeme[i] = getc (tape))) {
    // Se o primeiro caracter é 0, pode-se ter um número OCT ou HEX
    if (lexeme[i] == '0') {
      i++;

      // OCT: 0[0-7][0-7]*
      // Checa-se se o próximo caracter está entre 0 e 7
      if ('0' <= (lexeme[i] = getc (tape)) && lexeme[i] < '8') {
        i++;
        // Consomem-se todos os dígitos octais e retorna-se o primeiro caracter distinto
        while ('0' <= (lexeme[i] = getc (tape)) && lexeme[i] < '8') i++;
        ungetc (lexeme[i], tape);
        lexeme[i] = 0;
        // Retorna-se a classifcação OCT
        return OCT;
      }

      // Não é octal: devolve-se o caracter lido
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;

      // HEX: 0[xX][0-9A-Fa-f][0-9A-Fa-f]*
      // Se o próximo caracter após o 0 é um X, pode-se ter um hexadecimal
      if (toupper (lexeme[i] = getc (tape)) == 'X') {
        i++;

        // Se o próximo caracter é um dígito hexadecimal, continua-se lendo
        if (isxdigit (lexeme[i] = getc (tape))) {
          i++;

          while (isxdigit (lexeme[i] = getc (tape))) i++;
          // Devolver o primeiro caracter não hexadecimal encontrado
          ungetc (lexeme[i], tape);
          lexeme[i] = 0;
          // Retorna-se a classificação HEX
          return HEX;
        }
        // Não foi encontrado um xdigit, então devolve-se o caracter lido
        ungetc (lexeme[i], tape);
        lexeme[i] = 0;
        i--;
      }

      // Não é hexadecimal (não se encontrou X): devolver o caracter
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
    // Retorna-se a classificação DEC
      return DEC;
    }
    i++;

    // O primeiro caracter não era 0, portanto tem-se um número decimal comum.
    while (isdigit (lexeme[i] = getc (tape))) i++;
    // Após consumir todos os dígitos, devolve-se o caracter que não era um dígito
    ungetc (lexeme[i], tape);
    lexeme[i] = 0;
    // Retorna-se a classificação DEC
    return DEC;
  }
 
  // Não é um dígito: retorna-se o primeiro caracter lido
  ungetc (lexeme[i], tape);
  lexeme[i] = 0;
  return 0;
}

// EE = [eE] ['+''-']? [0-9][0-9]*
int isEE (FILE * tape)
{
  int i = strlen(lexeme);
  int pm;

  // [eE]
  // Verifica se encontrou o início de um exponencial
  if (toupper (lexeme[i] = getc (tape)) == 'E') {
    i++;

	  // ['+''-']?
    if ((lexeme[i] = getc (tape)) != '+' && lexeme[i] != '-') {
      // Não foi encontrado '+' ou '-': devolve-se o caracter lido
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
      pm = 0;
    } else {
      // Se encontrou-se '+' ou '-', a flag 'pm' é setada
      i++;
      pm = 1;
    }

	  // [0-9][0-9]*
    // Verifica se foi encontrado um dígito
    if (isdigit (lexeme[i] = getc (tape))) {
      i++;
      // São lidos todos os dígitos encontrados
      while (isdigit (lexeme[i] = getc (tape))) i++;
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
      // Retorna "true"
      return 1;
    }

    // Não foi encontrado um dígito: devolver o caracter lido
    ungetc (lexeme[i], tape);
    lexeme[i] = 0;
    i--;

    // Se foi encontrado um '+' ou '-', deve-se devolver mais um caracter
    if (pm) {
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
      i--;
    }
  }

  // Não foi encontrado um exponencial
  ungetc (lexeme[i], tape);
  lexeme[i] = 0;
  // Retorna "false"
  return 0;
}

// NUM = ( DEC | DEC '.'[0-9]* | '.'[0-9][0-9]* ) EE?
int isNUM (FILE * tape)
{
  int dec = isINT (tape);
  int i = strlen(lexeme);
  int token = 0;

  // Verifica-se o retorno de isINT, retornando-o caso seja OCT ou HEX
  switch (dec) {
	case OCT:
	case HEX:
		return dec;
  }

  // Se houve retorno de isINT, mas não era nem OCT nem HEX
  if (dec) {
    token = dec;
    // DEC '.'[0-9]*
    // Se após o DEC foi encontrado um '.'
    if ((lexeme[i] = getc (tape)) == '.') {
      i++;
      // Token classificado como FLT
      token = FLT;
      // São lidos todos os dígitos após o '.', devolvendo-se o próximo
      while (isdigit (lexeme[i] = getc (tape))) i++;
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
    } else {
      ungetc (lexeme[i], tape);
      lexeme[i] = 0;
    }
  } else {
	  // '.'[0-9][0-9]*
    // Não se tem um DEC: verifica-se se o próximo caracter é '.'
    if ((lexeme[i] = getc (tape)) == '.') {
      i++;
      if (isdigit (lexeme[i] = getc (tape))) {
        i++;
        // Token classificado como FLT
        token = FLT;
        // São lidos todos os dígitos após o '.', devolvendo-se o próximo
        while (isdigit (lexeme[i] = getc (tape))) i++;
        ungetc (lexeme[i], tape);
        lexeme[i] = 0;
      } else {
        // Não se encontrou um dígito após o '.': devolver caracteres lidos
        ungetc (lexeme[i], tape);
        lexeme[i] = 0;
        i--;
        ungetc (lexeme[i], tape);
        lexeme[i] = 0;
      }
    } else {
      // Não foi encontrado um '.': devolver caracter lido
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
  // Verifica-se se foi encontrado ':'
	if(lexeme[0] == ':') {
		lexeme[1] = getc(tape);
    // Verifica-se se foi encontrado '='
		if(lexeme[1] == '=') {
			lexeme[2] = 0;
      // Retorna-se ASGN
			return ASGN;
		}

    // Não foi encontrado '=': devolver caracter
		ungetc(lexeme[1], tape);
	}

  // Não foi encontrado ':': devolver caracter
	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
  // Retorna-se "false"
	return 0;
}

// GEQ (>=), NEQ (<>), LEQ (<=)
int isRELOP(FILE * tape) {
	lexeme[0] = getc(tape);
	lexeme[2] = 0;
	switch(lexeme[0]) {
      // Se encontrou-se '>', pode ser GEQ ou '>'
			case '>':
				lexeme[1] = getc(tape);
        // Se o próximo é '=', retorna-se GEQ
				if(lexeme[1] == '=') return GEQ;
        // Se não é, devolve o caracter e retorna '>'
				ungetc(lexeme[1], tape);
				lexeme[1] = 0;
				return '>';
      // Se encontrou-se '<', pode ser LEQ, NEQ ou '<'
			case '<':
				switch(lexeme[1] = getc(tape)) {
          // Se o próximo é '=', retorna-se LEQ
					case '=': return LEQ;
          // Se o próximo é '>', retorna-se NEQ
					case '>': return NEQ;
					default:
            // Se não é nenhum, devolve o caracter e retorna '<'
						ungetc(lexeme[1], tape);
						lexeme[1] = 0;
						return '<';
				}
	}

  // Se não era nem '>' nem '<', devolve o caracter
	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}

// Checa-se o tipo do token lido
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
