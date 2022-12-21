/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tokens.h>
#include <lexer.h>
#include <keywords.h>
#include <mypas.h>
#include <symtab.h>
#include <errors.h>
#include <parser.h>

int lookahead;
int symtab_vartype;
int symtab_sentinel;
int loopcount = 1;
int currentFunctionPos = -1;
extern int lineNumber;
extern FILE *source;
extern void match(int);

/** Gramática
 * 
 * mypas -> PROGRAM ID ( ID , ID ); declarative imperative .
 * declarative -> vardecl sbpdecl 
 * vardecl -> [ VAR idlist : vartype ; { idlist : vartype ; } ]
 * idlist -> ID { , ID }
 * vartype -> INTEGER | REAL | DOUBLE | BOOLEAN
 * sbpdecl -> { procedure | function }
 * procedure -> PROCEDURE ID parms ; declarative imperative ; 
 * function -> FUNCTION ID parms : vartype ; declarative imperative ;
 * parms -> [ ( [ VAR ] idlist : vartype { ; [ VAR ] idlist : vartype } ) ]
 * imperative -> BEGIN stmtlist END
 * stmtlist -> stmt { ; stmt }
 * stmt -> imperative | ifstmt | whilestmt | repstmt | idstmt | <empty>
 * ifstmt ->  IF expr THEN stmt [ ELSE stmt ]
 * expr -> smpexpr [ relop smpexpr ]
 * smpexpr -> [unary] term { addop term } 
 * relop -> '>' | GEQ | '<' | LEQ ! NEQ
 * unary -> '+' | '-' | NOT
 * addop -> '+' | '-' | OR
 * term -> fact { mulop fact } 
 * mulop -> '*' | '/' | DIV | MOD | AND
 * fact -> ID [ := expr | [ ( exprlist ) ] ] | DEC | FLT | TRUE | FALSE | ( expr )
 * exprlist -> expr { , expr }
 * whilestmt -> WHILE expr DO stmt
 * repstmt -> REPEAT stmtlist UNTIL expr
 * idstmt -> fact 
 */

// mypas -> PROGRAM ID ( ID , ID ) ; declarative imperative .
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
		// marcar a primeira posição da lista
		/**/symtab_first = symtab_next;/**/
		idlist();
		match(':');
		vartype();
		/**/ //definir o tipo para todas as vars de idlist
		symtab_set_type_for_range(symtab_vartype);
		/**/
		match(';');

		while(lookahead == ID) {
			/**/symtab_first = symtab_next;/**/
			idlist();
			match(':');
			vartype();
			/**/symtab_set_type_for_range(symtab_vartype);/**/
			match(';');
		}
	}
}

// idlist -> ID { , ID }
void idlist(void) {
	/**/symtab_append_and_set_objtype(lexeme, 2);/**/
	match(ID);
	
	while(lookahead == ',') {
		match(',');
		/**/symtab_append_and_set_objtype(lexeme, 2);/**/
		match(ID);
	}
}

// vartype -> INTEGER | REAL | DOUBLE | BOOLEAN
void vartype(void) {
	switch(lookahead) {
		case INTEGER:
			symtab_vartype = 1;
			match(lookahead); 
			break;
		case REAL:
			symtab_vartype = 2;
			match(lookahead); 
			break;
		case DOUBLE:
			symtab_vartype = 3;
			match(lookahead); 
			break;
		default:
			symtab_vartype = 4;
			match(BOOLEAN);
	}
}

// sbpdecl -> { procedure | function }
void sbpdecl(void) {
	while(lookahead == PROCEDURE || lookahead == FUNCTION) {
		// controle do lexlevel
		/**/lexlevel++;/**/
		if(lookahead == PROCEDURE) procedure();
		else function();
		/**/lexlevel--;/**/
	}
}

// procedure -> PROCEDURE ID parms ; declarative imperative ; 
void procedure(void) {
	match(PROCEDURE);
	/**/int pos = symtab_append_and_set_objtype(lexeme, 0);/**/
	/**/symtab_set_type(pos, 0);/**/
	match(ID);
	// marcar o next para depois que sair do contexto, sobrescrever a symtab
	/**/symtab_sentinel = symtab_next;/**/
	parms();
	match(';');
	declarative();
	imperative();
	match(';');
	// "release" na tabela após sair do contexto
	/**/symtab_next = symtab_sentinel;/**/
}

// function -> FUNCTION ID parms : vartype ; declarative imperative ;
void function(void) {
	match(FUNCTION);
	/**/int pos = symtab_append_and_set_objtype(lexeme, 1);/**/
	match(ID);
	
	// identificar a função atual para depois saber se pode fazer ':='
	/**/int previousFunctionPos = currentFunctionPos;/**/
	/**/currentFunctionPos = pos;/**/;

	/**/symtab_sentinel = symtab_next;/**/
	parms();
	match(':');
	vartype();
	/**/symtab_set_type(pos, symtab_vartype);/**/
	match(';');
	declarative();
	imperative();
	match(';');
	/**/symtab_next = symtab_sentinel;/**/

	// resetar a função atual ao sair da função
	/**/currentFunctionPos = previousFunctionPos;/**/
}

// parms -> [ ( [ VAR ] idlist : vartype { ; [ VAR ] idlist : vartype } ) ]
void parms(void) {
	if(lookahead == '(') {
		match('(');
		if(lookahead == VAR) match(VAR);
		/**/symtab_first = symtab_next;/**/
		idlist();
		match(':');
		vartype();
		// setar os tipos das variáveis recém-declaradas
		/**/symtab_set_type_for_range(symtab_vartype);/**/
		
		while(lookahead == ';') {
			match(';');
			if(lookahead == VAR) match(VAR);
			/**/symtab_first = symtab_next;/**/
			idlist();
			match(':');
			vartype();
			/**/symtab_set_type_for_range(symtab_vartype);/**/
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
	/**/int lbl1, lbl2;/**/

	match(IF);
	expr();
	match(THEN);

	//ação semântica 1 - desvio condicional se der 0
	/**/printf("\tjz .L%d\n", lbl1 = lbl2 = loopcount++);/**/

	stmt();
	if(lookahead == ELSE) {
		match(ELSE);
		
		//ação semântica 2 - jump
		/**/printf("\tjmp .L%d\n", lbl2 = loopcount++);/**/
		/**/printf(".L%d\n", lbl1);/**/

		stmt();
	}

	//ação semântica 3 - imprimir o rótulo de fim de estrutura
	/**/printf(".L%d\n", lbl2);/**/
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

// relop -> '>' | GEQ | '<' | LEQ ! NEQ
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
	/**/int pos;/**/
	/**/SYMTAB item;/**/

	switch(lookahead) {
		case ID:
			/**/pos = symtab_lookup(lexeme);/**/
			// se pos é -1 o símbolo não foi declarado
			/**/if(pos == -1) printUndeclaredSymbolError(lexeme);/**/
			match(ID);
			/**/item = symtab_get(pos);/**/

			if(lookahead == ASGN) {
				/**/ //se é proc OU se é function e não é a mesma function do contexto
				if(item.objtype == 0 || (item.objtype == 1 && pos != currentFunctionPos)){
					printInvalidAssignmentError(item.name);
				}
				/**/
				match(ASGN);
				expr();
			} else if(lookahead == '(') {
				// se é var, não pode chamar com ()
				/**/if(item.objtype == 2) printInvalidCallError(item.name);/**/
				match('(');
				exprlist();
				match(')');
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
	/**/int lbl1, lbl2;/**/
	/**/printf(".L%d\n", lbl1 = loopcount++);/**/
	match(WHILE);
	expr();
	match(DO);	
	/**/printf("\tjz .L%d\n", lbl2 = loopcount++);/**/
	stmt();
	/**/printf("\tjmp .L%d\n", lbl1);/**/
	/**/printf(".L%d\n", lbl2);/**/
}

// repstmt -> REPEAT stmtlist UNTIL expr
void repstmt(void) {
	/**/int lbl1;/**/
	/**/printf(".L%d\n", lbl1 = loopcount++);/**/
	
	match(REPEAT);
	stmtlist();
	match(UNTIL);
	expr();
	/**/printf("\tjz .L%d\n", lbl1);/**/
}

// idstmt -> fact 
void idstmt(void) {
	fact();
}

// match
void match(int expected) {
	if (expected == lookahead) {
		lookahead = gettoken(source);
	} else /**/printMismatchError(keyword[expected - BEGIN]);/**/
}
