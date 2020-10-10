%{

%}

COMMENT "^-^"..*
NUMBER [0-9]+\.?[0-9]*
VAR [a-zA-Z]+[0-9]*
SCAN "call"
SQRT "sqrt"
FLOAT "float"
PRINT "show"
INIT "START"
END "GAMEOVER"
%%


{FLOAT} {return FLOAT;}

{COMMENT} {return COMMENT;}

{SCAN} {return SCAN;}

{SQRT} {return SQRT;}

{PRINT} {return PRINT;}

{INIT} {return INIT;}			
	
{END} {return END;}

{NUMBER} {
	yylval.flo = atof(yytext);
	return NUMBER;
	}
	
{VAR} {
	strcpy (yylval.str,yytext);
	return VAR;
	}
	
[-+()=/*^] {return *yytext;}
	
[\ \n\t]*	{;}

. {;}
%%

int yywrap()
{
   return 1;
}