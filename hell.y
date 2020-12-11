%{

	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>
	#include <string.h>	
	#include "func.h"

	int yylex();
	void yyerror (char *s) {
		printf("%s\n", s);
	}

%}

%union {
	float flo;
	int fn;
	char str[50];
	Ast *a;
}

%token COMMENT
%token <flo>NUMBER
%token <str>VAR
%token <str>TEXT
%token IF ELSE WHILE
%token STRING INT FLOAT
%token SCANF SCANI SCANS
%token PRINTF PRINTI PRINTS
%token INIT 
%token END
%token <fn> CMP

%right '='
%left '+' '-'
%left '*' '/' '%'
%right '^' SQRT
%left CMP

%type <a> exp list stmt prog aString

%nonassoc IFX VARPREC VARPREC2 NEG VET declint declfloat declstring

%%

val: INIT prog END
	;

prog: stmt { eval($1); }
	| prog stmt { eval($2); }
	;
	
	
stmt: 
	IF '(' exp ')' '{' list '}' %prec IFX { $$ = newflow('I', $3, $6, NULL); }
	| IF '(' exp ')' '{' list '}' ELSE '{' list '}' { $$ = newflow('I', $3, $6, $10); }
	| WHILE '(' exp ')' '{' list '}' { $$ = newflow('W', $3, $6, NULL); }

	| VAR '=' exp %prec VARPREC { $$ = newasgn($1,$3); }
	| VAR '=' TEXT %prec VARPREC2 { $$ = newasgnS($1, newast('$', newValorValS($3), NULL)); }

	| INT VAR %prec declint { $$ = newvari('U',$2); }
	| FLOAT VAR %prec declfloat { $$ = newvari('V',$2); }
	| STRING VAR %prec declstring { $$ = newvari('X',$2); }

	| INT VAR '=' exp { $$ = newast('G', newvari('U',$2) , $4); }
	| FLOAT VAR '=' exp { $$ = newast('D', newvari('V',$2) , $4); }
	| STRING VAR '=' TEXT { $$ = newast('H', newvari('X',$2) , newValorValS($4)); }

	| SCANF '(' VAR ')' {  $$ = newvari('S', $3); }
	| SCANI '(' VAR ')' {  $$ = newvari('S', $3); }
	| SCANS '(' VAR ')' {  $$ = newvari('T', $3); }

	| PRINTF '(' exp')' { $$ = newast('p', $3, NULL); }
	| PRINTI '(' exp')' { $$ = newast('u', $3, NULL); }
	| PRINTS '(' aString ')' { $$ = $3; }
	;

aString: VAR { $$ = searchVar('z', $1); }
	| TEXT { $$ = newast('Z', newValorValS($1), NULL); }
	;

list: stmt { $$ = $1; }
		| list stmt { $$ = newast('L', $1, $2);	}
		;
	
exp: 
	exp '+' exp { $$ = newast('+', $1, $3); }
	| exp '-' exp { $$ = newast('-', $1, $3); }
	| exp '*' exp { $$ = newast('*', $1, $3); }
	| exp '/' exp { $$ = newast('/', $1, $3); }
	| exp '%' exp { $$ = newast('%', $1, $3); }
	| '(' exp ')' { $$ = $2; }
	| exp '^' exp { $$ = newast('^', $1, $3);  }
	| SQRT '(' exp ',' exp ')' { $$ = newast('~', $3, $5); }

	| exp CMP exp { $$ = newcmp($2, $1, $3); }
	| '-' exp %prec NEG { $$ = newast('M',$2,NULL); }
	| NUMBER { $$ = newnum($1); }
	| VAR	%prec VET { $$ = newValorVal($1); }
	;

;

%%

#include "lex.yy.c"

int main(){

	yyin = fopen("input.hell", "r");

	yyparse();
	yylex();
	fclose(yyin);
	
	return 0;
}

