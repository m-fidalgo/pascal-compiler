/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tokens.h>
#include <lexer.h>
#include <keywords.h>
#include <mypas.h>
#include <parser.h>

int lookahead;
extern FILE *source;
extern void match(int);

/** Gramática
 * 
 * mypas -> PROGRAM ID ( ID , ID ); declarative imperative .
 * declarative -> vardecl sbpdecl 
 * vardecl -> [ VAR idlist : vartype ; { idlist : vartype ; } ]
 * idlist -> ID { , ID }
 * vartype -> INTEGER | REAL | DOUBLE | BOOLEAN
 * sbpdecl -> { PROCEDURE ID parms ; declarative imperative ; | FUNCTION ID parms : vartype ; declarative imperative ; }
 * parms -> [ ( [ VAR ] idlist : vartype { ; [ VAR ] idlist : vartype } ) ]
 * imperative -> BEGIN stmtlist END
 * stmtlist -> stmt { ; stmt }
 * stmt -> imperative | ifstmt | whilestmt | repstmt | idstmt | <empty>
 * ifstmt ->  IF expr THEN stmt [ ELSE stmt ]
 * expr -> smpexpr [ relop smpexpr ]
 * smpexpr -> [unary] term { addop term } 
 * relop -> '>' ! GEQ | '<' | LEQ ! NEQ
 * unary -> '+' | '-' | NOT
 * addop -> '+' | '-' | OR
 * term -> fact { mulop fact } 
 * mulop -> '*' | '/' | DIV | MOD | AND
 * fact -> ID [ := expr | arg ] | DEC | FLT | TRUE | FALSE | ( expr )
 * arg -> [ ( exprlist ) ]
 * exprlist -> expr { , expr }
 * whilestmt -> WHILE expr DO stmt
 * repstmt -> REPEAT stmtlist UNTIL expr
 * idstmt -> fact 
 */

// mypas -> PROGRAM ID ( ID , ID ); declarative imperative .
void mypas(void) {
	match(PROGRAM);
	match(ID);
	match('(');
	match(ID);
	match(',');
	match(ID);
	match(')');
	match(';');
	declarative();
	imperative();
	match('.');
}

// declarative -> vardecl sbpdecl 
void declarative(void) {
	vardecl();
	sbpdecl();
}

// vardecl -> [ VAR idlist : vartype ; { idlist : vartype ; } ]
void vardecl(void) {
	if(lookahead == VAR) {
		match(VAR);
		idlist();
		match(':');
		vartype();
		match(';');

		while(lookahead == ID) {
			idlist();
			match(':');
			vartype();
			match(';');
		}
	}
}

// idlist -> ID { , ID }
void idlist(void) {
	match(ID);
	
	while(lookahead == ',') {
		match(',');
		match(ID);
	}
}

// vartype -> INTEGER | REAL | DOUBLE | BOOLEAN
void vartype(void) {
	switch(lookahead) {
		case INTEGER: match(INTEGER); break;
		case REAL: match(REAL); break;
		case DOUBLE: match(DOUBLE); break;
		case BOOLEAN: match(BOOLEAN); break;
		default: break;
	}
}

// sbpdecl -> { PROCEDURE ID parms ; declarative imperative ; 
//            | FUNCTION ID parms : vartype ; declarative imperative ; }
void sbpdecl(void) {
	while(lookahead == PROCEDURE || lookahead == FUNCTION) {
		if(lookahead == PROCEDURE) {
			match(PROCEDURE);
			match(ID);
			parms();
			match(';');
			declarative();
			imperative();
			match(';');
		} else {
			match(FUNCTION);
			match(ID);
			parms();
			match(':');
			vartype();
			match(';');
			declarative();
			imperative();
			match(';');
		}
	}
}

// parms -> [ ( [ VAR ] idlist : vartype { ; [ VAR ] idlist : vartype } ) ]
void parms(void) {
	if(lookahead == '(') {
		match('(');
		if(lookahead == VAR) match(VAR);
		idlist();
		match(':');
		vartype();
		
		while(lookahead == ';') {
			match(';');
			if(lookahead == VAR) match(VAR);
			idlist();
			match(':');
			vartype();
		}
		match(')');
	}
}

// imperative -> BEGIN stmtlist END
void imperative(void) {
	match(BEGIN);
	stmtlist();
	match(END);
}

// stmtlist -> stmt { ; stmt }
void stmtlist(void) {
	stmt();
	
	while(lookahead == ';') {
		match(';');
		stmt();
	}
}

// stmt -> imperative | ifstmt | whilestmt | repstmt | idstmt
void stmt(void) {
	switch(lookahead) {
		case BEGIN: imperative(); break;
		case IF: ifstmt(); break;
		case WHILE: whilestmt(); break;
		case REPEAT: repstmt(); break;
		case ID: idstmt(); break;
		default: ;
	}
}

// ifstmt ->  IF expr THEN stmt [ ELSE stmt ]
void ifstmt(void) {
	match(IF);
	expr();
	match(THEN);
	stmt();
	if(lookahead == ELSE) {
		match(ELSE);
		stmt();
	}
}

// expr -> smpexpr [ relop smpexpr ]
void expr(void) {
	smpexpr();
	if(relop()) {
		match(lookahead);
		smpexpr();
	}
}

// smpexpr -> [ unary ] term { addop term } 
void smpexpr(void) {
	if(unary()) match(lookahead);
	term();
	while(addop()) {
		match(lookahead);
		term();
	}
}

// relop -> '>' ! GEQ | '<' | LEQ ! NEQ
int relop(void) {
	switch(lookahead) {
		case '>': return '>';
		case GEQ: return GEQ;
		case '<': return '<';
		case LEQ: return LEQ;
		case NEQ: return NEQ;
	}
	return 0;
}

// unary -> '+' | '-' | NOT
int unary(void) {
	switch(lookahead) {
		case '+': return '+';
		case '-': return '-';
		case NOT: return NOT;
	}
	return 0;
}

// addop -> '+' | '-' | OR
int addop(void) {
	switch(lookahead) {
		case '+': return '+';
		case '-': return '-';
		case OR: return OR;
	}
	return 0;
}

// term -> fact { mulop fact } 
void term(void) {
	fact();
	while(mulop()) {
		match(lookahead);
		fact();
	}
}

// mulop -> '*' | '/' | DIV | MOD | AND
int mulop(void) {
	switch(lookahead) {
		case '*': return '*';
		case '/': return '/';
		case DIV: return DIV;
		case MOD: return MOD;
		case AND: return AND;
	}
	return 0;
}

// fact -> ID [ := expr | arg ] | DEC | FLT | TRUE | FALSE | ( expr )
void fact(void) {
	switch(lookahead) {
		case ID:
			match(ID);
			if(lookahead == ASGN) {
				match(ASGN);
				if(lookahead == '(') arg();
				else expr();
			}
			break;
		case DEC: match(DEC); break;
		case FLT: match(FLT); break;
		case TRUE: match(TRUE); break;
		case FALSE: match(FALSE); break;
		default: 
			match('(');
			expr();
			match(')');
			break;
	}
}

// arg -> [ ( exprlist ) ]
void arg(void) {
	if(lookahead == '(') {
		match('(');
		exprlist();
		match(')');
	}
}

// exprlist -> expr { , expr }
void exprlist(void) {
	expr();
	
	while(lookahead == ',') {
		match(',');
		expr();
	}
}

// whilestmt -> WHILE expr DO stmt
void whilestmt(void) {
	match(WHILE);
	expr();
	match(DO);
	stmt();
}

// repstmt -> REPEAT stmtlist UNTIL expr
void repstmt(void) {
	match(REPEAT);
	stmtlist();
	match(UNTIL);
	expr();
}

// idstmt -> fact 
void idstmt(void) {
	fact();
}

// match
void match(int expected) {
	if (expected == lookahead) {
		lookahead = gettoken(source);
	} else {
		fprintf(stderr, "token mismatch\n");
		exit(-2);
	}
}
