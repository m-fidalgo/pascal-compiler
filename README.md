<h1 align="center">Pascal Compiler</h1>
<p align="center"><img src="https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white" alt="C" /></p>

<h3>About</h3>
<p>The project was developed during the Compilers discipline offered by UNESP in 2022 as part of the Bachelor of Computer Science program.</p>

<h3>Grammar</h3>
<p>The compiler uses the following grammar:</p>

```
mypas -> PROGRAM ID ( ID , ID ) ; declarative imperative .
declarative -> vardecl sbpdecl
vardecl -> [ VAR idlist : vartype ; { idlist : vartype ; } ]
idlist -> ID { , ID }
vartype -> INTEGER | REAL | DOUBLE | BOOLEAN
sbpdecl -> { PROCEDURE ID parms ; declarative imperative ; | FUNCTION ID parms : vartype ; declarative imperative ; }
parms -> [ ( [ VAR ] idlist : vartype { ; [ VAR ] idlist : vartype } ) ]
imperative -> BEGIN stmtlist END
stmtlist -> stmt { ; stmt }
stmt -> imperative | ifstmt | whilestmt | repstmt | idstmt | <empty>
ifstmt ->  IF expr THEN stmt [ ELSE stmt ]
expr -> smpexpr [ relop smpexpr ]
smpexpr -> [unary] term { addop term }
relop -> '>' | GEQ | '<' | LEQ ! NEQ
unary -> '+' | '-' | NOT
addop -> '+' | '-' | OR
term -> fact { mulop fact }
mulop -> '*' | '/' | DIV | MOD | AND
fact -> ID [ := expr | arg ] | DEC | FLT | TRUE | FALSE | ( expr )
arg -> [ ( exprlist ) ]
exprlist -> expr { , expr }
whilestmt -> WHILE expr DO stmt
repstmt -> REPEAT stmtlist UNTIL expr
idstmt -> fact
```
