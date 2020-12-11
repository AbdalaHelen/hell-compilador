#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// inteiro - 0
// real - 1
// texto - 2

// estrutura de uma variável
typedef struct vars { 
    int nodetype;
    char name[50];
    double valor;
    char valors[1000];
    double *vet;
    struct vars *prox;
} VARI;

//nova variável FLOAT
VARI *ins_f(VARI *l, char n[]) {
  VARI *neww = (VARI *)malloc(sizeof(VARI));
  
  strcpy(neww->name, n);
  neww->prox = l;
  neww->nodetype = 1;

  return neww;
}

// nova variável INT
VARI *ins_i(VARI *l, char n[]) {
  VARI *new = (VARI *)malloc(sizeof(VARI));
  
  strcpy(new->name, n);
  new->prox = l;
  new->nodetype = 0;
  
  return new;
}

// nova variável STRING
VARI *ins_s(VARI *l, char n[]) {
  VARI *new = (VARI *)malloc(sizeof(VARI));

  strcpy(new->name, n);
  new->prox = l;
  new->nodetype = 2;
  
  return new;
}

// busca uma variável na lista de variáveis
VARI *srch(VARI *l, char n[]) {
  VARI *aux = l;

  while(aux != NULL) {
    if(strcmp(n, aux->name) == 0)
      return aux;
    aux = aux->prox;
  }

  return aux;
}

typedef struct ast {
  int nodetype;
  struct ast *l;
  struct ast *r;
} Ast;

// Estrutura de um número
typedef struct numval {
  int nodetype;
  double valor;
} Numval;

// Estrutura de um nome de variável, nesse exemplo uma variável é um número no vetor var[26]
typedef struct varval {
  int nodetype;
  char var[50];
  int size;
  Ast *length;
} Varval;

// Estrutura de um texto
typedef struct texto {
  int nodetype;
  char txt[50];
} TXT;

// Estrutura de um desvio (if/else/while)
typedef struct flow {
  int nodetype;
  Ast *cond; // condição
  Ast *tl;   // then, ou seja, verdade
  Ast *el;   // else
} Flow;

// Estrutura para um nó de atribuição. Para atrubior o valor de v em s
typedef struct symasgn {
  int nodetype;
  char s[50];
  Ast *v;
  int pos;
  Ast *indice;
} Symasgn;

// Variáveis
VARI *l1; 
VARI *aux;

// Função para criar um nó
Ast *newast(int nodetype, Ast *l, Ast *r) { 
  Ast *a = (Ast *)malloc(sizeof(Ast));

  if (!a) {
    printf("out of space");
    exit(0);
  }
  a->nodetype = nodetype;
  a->l = l;
  a->r = r;

  return a;
}

// Função de que cria uma nova variável
Ast *newvari(int nodetype, char nome[50]) {
  Varval *a = (Varval *)malloc(sizeof(Varval));

  if (!a) {
    printf("out of space");
    exit(0);
  }
  a->nodetype = nodetype;
  strcpy(a->var, nome);

  return (Ast *)a;
}

// Função de que cria um novo número
Ast *newnum(double d) {
  Numval *a = (Numval *)malloc(sizeof(Numval));

  if(!a) {
    printf("out of space");
    exit(0);
  }
  a->nodetype = 'K';
  a->valor = d;

  return (Ast *)a;
}

// Função que cria um nó de if/else/while
Ast *newflow(int nodetype, Ast *cond, Ast *tl, Ast *el) {
  Flow *a = (Flow *)malloc(sizeof(Flow));

  if(!a) {
      printf("out of space");
      exit(0);
  }
  a->nodetype = nodetype;
  a->cond = cond;
  a->tl = tl;
  a->el = el;

  return (Ast *)a;
}

// Função que cria um nó para testes
Ast *newcmp(int cmptype, Ast *l, Ast *r) {
  Ast *a = (Ast *)malloc(sizeof(Ast));
  
  if(!a) {
      printf("out of space");
      exit(0);
  }
  a->nodetype = '0' + cmptype;
  a->l = l;
  a->r = r;
  
  return a;
}

// Função para um nó de atribuição
Ast *newasgn(char s[50], Ast *v) {
  Symasgn *a = (Symasgn *)malloc(sizeof(Symasgn));
  if(!a) {
    printf("out of space");
    exit(0);
  }
  a->nodetype = '=';
  strcpy(a->s, s);
  a->v = v; // Valor
  
  return (Ast *)a;
}

// Função para um nó de atribuição
Ast *newasgnS(char s[50], Ast *v) { 
  Symasgn *a = (Symasgn *)malloc(sizeof(Symasgn));
  if (!a) {
    printf("out of space");
    exit(0);
  }
  a->nodetype = 'a';
  strcpy(a->s, s);
  a->v = v; 
 
  return (Ast *)a;
}

// Função que recupera o nome/referência de uma variável, neste caso o número
Ast *newValorVal(char s[]) {
  Varval *a = (Varval *)malloc(sizeof(Varval));
  
  if(!a) {
    printf("out of space");
    exit(0);
  }
  a->nodetype = 'N';
  strcpy(a->var, s);

  return (Ast *)a;
}
// Função que recupera o nome/referência de uma variável, neste caso o número
Ast *newValorValS(char s[50]) { 
  Varval *a = (Varval *)malloc(sizeof(Varval));

  if(!a) {
    printf("out of space");
    exit(0);
  }
  a->nodetype = 'Q';
  strcpy(a->var, s);

  return (Ast *)a;
}

Ast *searchVar(int nodetype, char s[50]) {
  Symasgn *a = (Symasgn *)malloc(sizeof(Symasgn));

  if (!a) {
    printf("out of space");
    exit(0);
  }
  a->nodetype = nodetype;
  strcpy(a->s, s);

  return (Ast *)a;
}

// Função que executa operações a partir de um nó
char *eval2(Ast *a) {
  VARI *aux1;
  char *v2;

  switch(a->nodetype) {
    case 'Q':
      aux1 = srch(l1, ((Varval *)a)->var);
      return aux1->valors;
      break;

    default:
      printf("EVAL2: internal error: bad node %c\n", a->nodetype);
      break;
  }

  return v2;
}

double evalAux(Ast *a) {
  double v;
  double value;
  char v1[50];
  char *v2;
  VARI *aux1;

  if(!a) {
    printf("internal error, null eval");
    return 0.0;
  }

  switch(a->nodetype) {
    case 'K': // Recupera um número
      v = ((Numval *)a)->valor;
      break; 
    case 'N':
      aux1 = srch(l1, ((Varval *)a)->var);
      v = aux1->valor;
      break;

    case 'n':
      aux1 = srch(l1, ((Varval *)a)->var);
      v = aux1->vet[((Varval *)a)->size];
      break;

    case '+':
      v = evalAux(a->l) + evalAux(a->r);
      break; 
    case '-':
      v = evalAux(a->l) - evalAux(a->r);
      break; 
    case '*':
      v = evalAux(a->l) * evalAux(a->r);
      break; 
    case '/':
      v = evalAux(a->l) / evalAux(a->r);
      break;
    case '%':
      v = (int) evalAux(a->l) % (int) evalAux(a->r);
      break;
    case '^':
      v = pow(evalAux(a->l), evalAux(a->r));
      break;
    case '~':
      v = pow(evalAux(a->l), (1 / evalAux(a->r)));
      break;
    case 'M':
      v = -evalAux(a->l);
      break;

    case '1': // Operações lógicas. "árv esq   >   árv dir"  Se verdade 1, falso 0
      v = (evalAux(a->l) > evalAux(a->r)) ? 1 : 0;
      break; 
    case '2':
      v = (evalAux(a->l) < evalAux(a->r)) ? 1 : 0;
      break;
    case '3':
      v = (evalAux(a->l) != evalAux(a->r)) ? 1 : 0;
      break;
    case '4':
      v = (evalAux(a->l) == evalAux(a->r)) ? 1 : 0;
      break;
    case '5':
      v = (evalAux(a->l) >= evalAux(a->r)) ? 1 : 0;
      break;
    case '6':
      v = (evalAux(a->l) <= evalAux(a->r)) ? 1 : 0;
      break;
    default:
      printf("EVAL1: internal error: bad node %c\n", a->nodetype);
      break;
  }

  return v;
}

char *parseSTR(Ast *a) {
  char *v = (char *)malloc(1000 * sizeof(char));
  double value;
  VARI *aux1;

  if(!a) {
    printf("internal error, null eval");
    return NULL;
  }

  switch(a->nodetype) {
    case '$':
      return ((TXT *)a->l)->txt;
      break;
    case '7':
      value = evalAux(a->l);
      sprintf(v, "%.2lf", value);
      break;
    case '8':
      value = evalAux(a->l);
      sprintf(v, "%.0lf", value);
      break;
    case '9':
      aux1 = srch(l1, ((Symasgn *)(a->l))->s);
      if (aux1->nodetype == 2)
        return aux1->valors;
      else
        printf("Variável não é do tipo String %c\n", a->nodetype);
      break;
    default:
      printf("STRING: internal error: bad node %c\n", a->nodetype);
      break;
  }

  return v;
}

// Função que executa operações a partir de um nó
double eval(Ast *a) {
  double v;
  double value;
  char v1[50];
  char *v2;
  VARI *aux1;

  if(!a) {
    printf("internal error, null eval");
    return 0.0;
  }

  switch(a->nodetype) {
    case 'K':
      v = ((Numval *)a)->valor;
      break; /*Recupera um número*/
    case 'N':
      aux1 = srch(l1, ((Varval *)a)->var);
      v = aux1->valor;
      break;
    case 'n':
      aux1 = srch(l1, ((Varval *)a)->var);
      value = eval(((Varval *)a)->length);
      v = aux1->vet[(int)value];
      break;

    case '+':
      v = eval(a->l) + eval(a->r);
      break;
    case '-':
      v = eval(a->l) - eval(a->r);
      break;
    case '*':
      v = eval(a->l) * eval(a->r);
      break;
    case '/':
      v = eval(a->l) / eval(a->r);
      break;
    case '%':
      v = (int)eval(a->l) % (int)eval(a->r);
      break;
    case '^':
      v = pow(eval(a->l), eval(a->r));
      break;
    case '~':
      v = pow(eval(a->l), (1 / eval(a->r)));
      break;
    case 'M':
      v = -eval(a->l);
      break;

    case '1':
      v = (eval(a->l) > eval(a->r)) ? 1 : 0;
      break;
    case '2':
      v = (eval(a->l) < eval(a->r)) ? 1 : 0;
      break;
    case '3':
      v = (eval(a->l) != eval(a->r)) ? 1 : 0;
      break;
    case '4':
      v = (eval(a->l) == eval(a->r)) ? 1 : 0;
      break;
    case '5':
      v = (eval(a->l) >= eval(a->r)) ? 1 : 0;
      break;
    case '6':
      v = (eval(a->l) <= eval(a->r)) ? 1 : 0;
      break;

    case '=':
      v = eval(((Symasgn *)a)->v); /*Recupera o valor*/
      aux = srch(l1, ((Symasgn *)a)->s);
      aux->valor = v;
      break;

    case 'a':
      //v = eval(((Symasgn *)a)->v); /*Recupera o valor*/
      strcpy(v1,  parseSTR(((Symasgn *)a)->v));
      aux = srch(l1, ((Symasgn *)a)->s);
      strcpy(aux->valors, v1);
      break;
      
    case 'I': /*CASO IF*/
      if(eval(((Flow *)a)->cond) != 0) { // executa a condição / teste
        if (((Flow *)a)->tl)            // Se existir árvore
          v = eval(((Flow *)a)->tl);    // Verdade
        else
          v = 0.0;
      } else {
        if(((Flow *)a)->el) {
          v = eval(((Flow *)a)->el); /*Falso*/
        } else
          v = 0.0;
      }
      break;

    case 'W':
      v = 0.0;
      if(((Flow *)a)->tl) {
        while(eval(((Flow *)a)->cond) != 0) {
          v = eval(((Flow *)a)->tl);
        }
      }
      break;

    case 'L':
      eval(a->l);
      v = eval(a->r);
      break; // Lista de operções em um bloco IF/ELSE/WHILE. Assim o analisador não se perde entre os blocos

    case 'P':
      v = eval(a->l); // Imprime um FLOAT
      printf("%.2f\n", v);
      break;

    case 'S':
      fflush(stdin);
      scanf("%lf", &v);
      fflush(stdin);
      aux1 = srch(l1, ((Varval *)a)->var);
      aux1->valor = v;
      break;

    case 'T':
      fflush(stdin);
      scanf("%s", v1);
      fflush(stdin);
      aux1 = srch(l1, ((Varval *)a)->var);
      strcpy(aux1->valors, v1);
      break;

    case 'Y':
      v2 = eval2(a->l); /*Recupera um valor STR*/
      printf("%s\n", v2);
      break; /*Função que imprime um valor (string)*/

    /** =========  Função que imprime um valor (string) ============ */
    // subdivisao do float
    case 'q':
      eval(a->r);
      eval(a->l);
      /*Recupera um valor STR*/
      break; /*Função que imprime um valor (string)*/
    case 'p':
      value = eval(a->l); /*Recupera um valor STR*/
      printf("%.2lf\n", value);
      break;

    // subdivisao do int
    case 'v':
      eval(a->r);
      eval(a->l); // Recupera um valor STR
      break; // Função que imprime um valor (string)
    case 'u':
      value = eval(a->l);
      printf("%d\n", (int)value);
      break;

    case 'z':
      aux1 = srch(l1, ((Varval *)a)->var);
      if (aux1->nodetype == 2)
        printf("%s\n", aux1->valors);
      else
        printf("Variável não é do tipo String %c\n", a->nodetype);
      break;

    case 'Z':
      printf("%s\n", ((TXT*)a->l)->txt);
      break;

    case 'V': //armazena um float
      l1 = ins_f(l1, ((Varval *)a)->var);
      break;
    case 'U': //armazena um int
      l1 = ins_i(l1, ((Varval *)a)->var);
      break;
    case 'X': //armazena um string
      l1 = ins_s(l1, ((Varval *)a)->var);
      break;

    case 'D': //declarar e inserir float
      v = eval(a->r);
      l1 = ins_f(l1, ((Symasgn *)(a->l))->s);
      l1->valor = v;
      break;
    case 'd': //declarar varios float
      l1 = ins_f(l1, ((Symasgn *)(a->l))->s);
      eval(a->r);
      break;

    case 'G': //declarar e inserir int
      v = eval(a->r);
      l1 = ins_i(l1, ((Symasgn *)(a->l))->s);
      l1->valor = v;
      break;
    case 'g': //declarar varios int
      l1 = ins_i(l1, ((Symasgn *)(a->l))->s);
      eval(a->r);
      break;

    case 'H': //declarar e inserir string
      l1 = ins_s(l1, (((Symasgn *)(a->l))->s));
      strcpy(l1->valors, ((Varval *)a->r)->var);
      break;
    case 'h': //declarar varios string
      l1 = ins_s(l1, ((Symasgn *)(a->l))->s);
      eval(a->r);
      break;
    case '@':
      v2 = parseSTR(((Symasgn *)a)->v);
      aux1 = srch(l1, ((Symasgn *)a)->s);
      
      if (aux1->nodetype == 2)
        strcpy(aux1->valors, v2);
      else
        printf("Variável %s não é do tipo String \n", ((Symasgn *)a)->s);
      break;
    case '#':
      v2 = parseSTR(((Symasgn *)a)->v);
      aux1 = srch(l1, ((Symasgn *)a)->s);
      if (aux1->nodetype == 2)
        strcat(aux1->valors, v2);
      else
          printf("Variável %s não é do tipo String \n", ((Symasgn *)a)->s);
      break;
    default:
      printf("EVAL1: internal error: bad node %c\n", a->nodetype);
      break;
  }
  
  return v;
}
