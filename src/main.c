#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
extern int   yyparse(void);
extern void  print_symbol_table(void);

int print_tokens = 0;
int print_symtab = 0;
int emit_3ac     = 0;
int emit_asm     = 0;
int lineno       = 1;

int main(int argc, char **argv) {
    const char *input_file = NULL;

    for (int i = 1; i < argc; i++) {
        if      (strcmp(argv[i], "-tokens") == 0) print_tokens = 1;
        else if (strcmp(argv[i], "-symtab") == 0) print_symtab = 1;
        else if (strcmp(argv[i], "-3ac")    == 0) emit_3ac     = 1;
        else if (strcmp(argv[i], "-asm")    == 0) emit_asm     = 1;
        else input_file = argv[i];
    }

    if (!input_file) {
        fprintf(stderr, "Usage: %s <input.c> [-tokens] [-symtab] [-3ac] [-asm]\n", argv[0]);
        return 1;
    }

    yyin = fopen(input_file, "r");
    if (!yyin) {
        fprintf(stderr, "Cannot open file: %s\n", input_file);
        return 1;
    }

    if (print_tokens) printf("TOKEN STREAM\n------------\n");

    if (yyparse() == 0) {
        if (print_symtab) print_symbol_table();
    }

    fclose(yyin);
    return 0;
}
