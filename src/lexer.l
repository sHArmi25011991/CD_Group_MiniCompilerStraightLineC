%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

int  yylex(void);
void yyerror(const char *s);

extern int print_tokens;
extern int lineno;

static void show_token(const char *name, const char *lexeme) {
    if (print_tokens)
        printf("<%s, %s>\n", name, lexeme ? lexeme : "");
}
%}

%option noyywrap

%%
"+"             { show_token("PLUS",      "+");  return PLUS;      }
"-"             { show_token("MINUS",     "-");  return MINUS;     }
"*"             { show_token("MUL",       "*");  return MUL;       }
"/"             { show_token("DIV",       "/");  return DIV;       }
"="             { show_token("ASSIGN",    "=");  return ASSIGN;    }
"("             { show_token("LPAREN",    "(");  return LPAREN;    }
")"             { show_token("RPAREN",    ")");  return RPAREN;    }
";"             { show_token("SEMICOLON", ";");  return SEMICOLON; }

[0-9]+              {
                        show_token("NUMBER", yytext);
                        yylval.ival = atoi(yytext);
                        return NUMBER;
                    }

[a-zA-Z_][a-zA-Z0-9_]* {
                        show_token("IDENTIFIER", yytext);
                        yylval.sval = strdup(yytext);
                        return IDENTIFIER;
                    }

[ \t\r]+        ;
\n              { lineno++; }

.               {
                    fprintf(stderr,
                        "Lexical Error at line %d: illegal character '%s'\n",
                        lineno, yytext);
                }
%%
