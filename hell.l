%{
#include <string.h>
%}

COMMENT "^-^".*
NUMBER [0-9]+\.?[0-9]*
VAR [a-zA-Z]+[0-9]*
SQRT "sqrt"

FLOAT "float"
INT "int"
STRING "string"
TEXT ["][^"]*["]

INIT "START"
END "GAMEOVER"

IF "if"
ELSE "else"
WHILE "while"

PRINTF "showf"
PRINTI "showi"
PRINTS "shows"
SCANF "callf"
SCANI "calli"
SCANS "calls"

%%
{INIT} {return INIT;}
{END} {return END;}

{COMMENT} { /* ignore */ }

{IF} {return IF;}
{ELSE} {return ELSE;}
{WHILE} {return WHILE;}

{SQRT} { return SQRT; }

{SCANF} {return SCANF;}
{SCANI} {return SCANI;}
{SCANS} {return SCANS;}
{PRINTF} {return PRINTF;}
{PRINTI} {return PRINTI;}
{PRINTS} {return PRINTS;}

{FLOAT} {return FLOAT;}
{INT} {return INT;}
{STRING} {return STRING;}

{VAR} {
	strcpy(yylval.str,yytext);
	return VAR;
}

{TEXT} {
	char aux[50];
	strncpy ( aux, &yytext[1], strlen(yytext)-2);
	aux[strlen(yytext)-2]='\0';
	strcpy (yylval.str, aux);

	return TEXT;  
}

{NUMBER} {
	yylval.flo = atof(yytext);
	return NUMBER;
}

[-+()=/*^{};] {return *yytext;}

">" {yylval.fn = 1; return CMP; }
"<" {yylval.fn = 2; return CMP; }
"!=" {yylval.fn = 3; return CMP; }
"==" {yylval.fn = 4; return CMP; }
">=" {yylval.fn = 5; return CMP; }
"<=" {yylval.fn = 6; return CMP; }

[\ \n\t]*	{;}
. { /* ignore */}
%%

int yywrap() {
   return 1;
}