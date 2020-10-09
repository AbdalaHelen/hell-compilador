%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int yylex();
void yyerror (char *s){
	printf("%s\n", s);
}

	typedef struct variable{
		char name[50];
		float value;
		struct variable * prox;
	}VARIABLE;
	
	//inserts a new variable in the variable list
	VARIABLE * ins(VARIABLE*l,char n[]){
		VARIABLE*new =(VARIABLE*)malloc(sizeof(VARIABLE));
		strcpy(new->name,n);
		new->prox = l;
		return new;
	}
	
	//search for a variable in the variable list
	VARIABLE *srch(VARIABLE*l,char n[]){
		VARIABLE*aux = l;
		while(aux != NULL){
			if(strcmp(n,aux->name)==0)
				return aux;
			aux = aux->prox;
		}
		return aux;
	}
	
	VARIABLE *l1;
%}

%union{
	float flo;
	char str[50];
	}

%token <flo>NUMBER
%token <str>VAR
%token SCAN
%token SQRT
%token FLOAT
%token PRINT
%token END
%token INIT
%left '+' '-' 
%left '*' '/'
%right '^'
%right NEG
%type <flo> exp
%type <flo> value


%%


prog: INIT cod END
	;

cod: cod cmdos
	|
	;

cmdos: SCAN '(' VAR ')' {
            float aux;
            printf ("Enter a value: ");
            scanf ("%f", &aux);
            VARIABLE * aux1 = srch(l1, $3);
            if(aux1 == NULL) {
                printf("Variable not declared: %s\n", $3);
            }
            else {
                aux1 -> value = aux;
            }
        }
        |FLOAT VAR {
					VARIABLE * aux = srch(l1,$2);
					if (aux == NULL)
						l1 = ins(l1,$2);
					else
						printf ("Variable redeclaration: %s\n",$2);
				 	 }
	|
	
		PRINT '(' exp ')' {
						printf ("%.2f \n",$3);
						}
	| 	
		VAR '=' exp {
					VARIABLE * aux = srch(l1,$1);
					if (aux == NULL)
						printf ("Variable not declared: %s\n",$1);
					else
						aux -> value = $3;
		}
    |
        SQRT '(' VAR ')' {
                    VARIABLE * aux = srch(l1,$3);
					if (aux == NULL)
						printf ("Variable not declared: %s\n",$3);
					else
                        printf("Square root of %s is = %.2f", $3, sqrt(aux -> value));
        }
	;

exp: exp '+' exp {$$ = $1 + $3;}
	|exp '-' exp {$$ = $1 - $3;}
	|exp '*' exp {$$ = $1 * $3;}
	|exp '/' exp {$$ = $1 / $3;}
	|'(' exp ')' {$$ = $2;}
	|exp '^' exp {$$ = pow($1,$3);}
	|'-' exp %prec NEG {$$ = -$2;}
	|value {$$ = $1;}
	|VAR {
			VARIABLE * aux = srch (l1,$1);
			if (aux == NULL)
				printf ("Variable not declared: %s\n",$1);
			else
				$$ = aux->value;
			}
    |SQRT '(' VAR ')' {
        VARIABLE* aux = srch(l1, $3);
        if(aux == NULL) 
            printf("Variable not declared: %s\n", $3);
        else
            $$ = sqrt(aux -> value);
    }
	;

value: NUMBER {$$ = $1;}
	;

%%

#include "lex.yy.c"

int main(){
	l1 = NULL;
	yyin=fopen("input.hell","r");
	yyparse();
	yylex();
	fclose(yyin);
return 0;
}
