%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir.h"
#include "codegen.h"

int  yylex(void);
void yyerror(const char *s);

extern int lineno;
extern FILE *yyin;
extern int emit_3ac;
extern int emit_asm;

/* ---------- Symbol Table ---------- */
typedef struct {
    char name[64];
    char type[16];
    int  value;
    int  defined;
} SymbolEntry;

SymbolEntry symtab[100];
int         sym_count = 0;

SymbolEntry *lookup(const char *name) {
    for (int i = 0; i < sym_count; i++)
        if (strcmp(symtab[i].name, name) == 0)
            return &symtab[i];
    return NULL;
}

SymbolEntry *add_symbol(const char *name, const char *type) {
    SymbolEntry *e = lookup(name);
    if (e) return e;
    if (sym_count >= 100) {
        fprintf(stderr, "Symbol table overflow\n");
        exit(1);
    }
    e = &symtab[sym_count++];
    strncpy(e->name, name, sizeof(e->name) - 1);
    strncpy(e->type, type, sizeof(e->type) - 1);
    e->value   = 0;
    e->defined = 1;
    return e;
}

void print_symbol_table(void) {
    printf("\nSYMBOL TABLE\n------------\n");
    printf("Name    Type    Value\n");
    for (int i = 0; i < sym_count; i++) {
        printf("%-7s %-7s %d\n",
               symtab[i].name, symtab[i].type, symtab[i].value);
    }
}

/* ---------- IR list ---------- */
static IRList ir;
static int    temp_count = 0;

static void emit_ir(IRType type, const char *dest,
                    char op, const char *a1, const char *a2) {
    if (ir.count >= MAX_IR) return;
    IRInstr *ins = &ir.items[ir.count++];
    ins->type = type;
    ins->op   = op;
    strncpy(ins->dest, dest, sizeof(ins->dest) - 1);
    strncpy(ins->arg1, a1,   sizeof(ins->arg1) - 1);
    strncpy(ins->arg2, a2,   sizeof(ins->arg2) - 1);
}

static void new_temp_name(char *buf) {
    sprintf(buf, "t%d", ++temp_count);
}
%}

%union {
    int   ival;
    char *sval;
    char *place;
}

%token <sval>  IDENTIFIER
%token <ival>  NUMBER
%token ASSIGN PLUS MINUS MUL DIV LPAREN RPAREN SEMICOLON

%type <place> expr term factor

%start program

%%
program:
      stmt_list
      {
          if (emit_3ac) {
              printf("THREE-ADDRESS CODE\n------------------\n");
              print_ir(&ir);
          }
          if (emit_asm) {
              printf("\nASSEMBLY (R0-R3)\n----------------\n");
              generate_assembly(&ir);
          }
      }
    ;

stmt_list:
      stmt
    | stmt_list stmt
    ;

stmt:
      IDENTIFIER ASSIGN expr SEMICOLON   {
            add_symbol($1, "int");
            emit_ir(IR_ASSIGN, $1, 0, $3, "");
            free($3);
      }
    ;

expr:
      expr PLUS  term   {
            char t[32]; new_temp_name(t);
            emit_ir(IR_BINOP, t, '+', $1, $3);
            $$ = strdup(t);
            free($1); free($3);
      }
    | expr MINUS term   {
            char t[32]; new_temp_name(t);
            emit_ir(IR_BINOP, t, '-', $1, $3);
            $$ = strdup(t);
            free($1); free($3);
      }
    | term              { $$ = $1; }
    ;

term:
      term MUL factor   {
            char t[32]; new_temp_name(t);
            emit_ir(IR_BINOP, t, '*', $1, $3);
            $$ = strdup(t);
            free($1); free($3);
      }
    | term DIV factor   {
            /* Semantic check: Division by zero */
            if ($3[0] >= '0' && $3[0] <= '9' && atoi($3) == 0) {
                fprintf(stderr,
                  "Semantic Error at line %d: division by zero\n",
                  lineno);
            }
            char t[32]; new_temp_name(t);
            emit_ir(IR_BINOP, t, '/', $1, $3);
            $$ = strdup(t);
            free($1); free($3);
      }
    | factor            { $$ = $1; }
    ;

factor:
      LPAREN expr RPAREN { $$ = $2; }
    | IDENTIFIER         {
            /* Semantic check: Undeclared variable */
            if (!lookup($1)) {
                fprintf(stderr,
                  "Semantic Error at line %d: variable '%s' undeclared\n",
                  lineno, $1);
            }
            $$ = strdup($1);
      }
    | NUMBER             {
            char buf[32]; sprintf(buf, "%d", $1);
            $$ = strdup(buf);
      }
    ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", lineno, s);
}
