%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int  yylex(void);
void yyerror(const char *s);

extern int lineno;
extern FILE *yyin;
%}

%union {
    int   ival;
    char *sval;
}

%token <sval> IDENTIFIER
%token <ival> NUMBER
%token ASSIGN PLUS MINUS MUL DIV LPAREN RPAREN SEMICOLON

%start program

%%
program:
      stmt_list
    ;

stmt_list:
      stmt
    | stmt_list stmt
    ;

stmt:
      IDENTIFIER ASSIGN expr SEMICOLON   {
            free($1);
      }
    ;

expr:
      expr PLUS  term
    | expr MINUS term
    | term
    ;

term:
      term MUL factor
    | term DIV factor
    | factor
    ;

factor:
      LPAREN expr RPAREN
    | IDENTIFIER         { free($1); }
    | NUMBER
    ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", lineno, s);
}
