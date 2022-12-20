/**@<parser.h>::**/

// Definindo cabeçalho de funções
extern void match(int);
extern void declarative(void);
extern void vardecl(void);
extern void idlist(void);
extern void vartype(void);
extern void sbpdecl(void);
extern void procedure(void);
extern void function(void);
extern void parms(void);
extern void imperative(void);
extern void stmtlist(void);
extern void stmt(void);
extern void ifstmt(void);
extern void expr(void);
extern void smpexpr(void);
extern int relop(void);
extern int unary(void);
extern int addop(void);
extern void term(void);
extern int mulop(void);
extern void fact(void);
extern void exprlist(void);
extern void whilestmt(void);
extern void repstmt(void);
extern void idstmt(void);

