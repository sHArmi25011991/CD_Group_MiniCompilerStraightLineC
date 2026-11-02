#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
 * External declarations from Lexer and Parser.
 * These are defined in lexer.l and parser.y
 */
extern FILE *yyin;
extern int   yyparse(void);
extern void  print_symbol_table(void);

/* 
 * Global flags to control the output of different stages.
 * Initialized to 0 (false/off) by default.
 */
int print_tokens = 0;  /* Phase 1: -tokens */
int print_symtab = 0;  /* Phase 2: -symtab */
int emit_3ac     = 0;  /* Phase 2: -3ac    */
int emit_asm     = 0;  /* Phase 2: -asm    */
int emit_opt     = 0;  /* Phase 3: -opt    */

/* Line number counter for accurate error reporting */
int lineno       = 1;

int main(int argc, char **argv) {
    const char *input_file = NULL;

    /* 
     * Loop through command line arguments to parse flags and find the input file.
     * We start from i = 1 because argv[0] is the program name (./compiler).
     */
    for (int i = 1; i < argc; i++) {
        if      (strcmp(argv[i], "-tokens") == 0) print_tokens = 1;
        else if (strcmp(argv[i], "-symtab") == 0) print_symtab = 1;
        else if (strcmp(argv[i], "-3ac")    == 0) emit_3ac     = 1;
        else if (strcmp(argv[i], "-asm")    == 0) emit_asm     = 1;
        else if (strcmp(argv[i], "-opt")    == 0) emit_opt     = 1;
        else input_file = argv[i]; /* If it's not a flag, it must be the filename */
    }

    /* Error checking: If no input file was provided, show usage instructions */
    if (!input_file) {
        fprintf(stderr, "Usage: %s <input.c> [-tokens] [-symtab] [-3ac] [-asm] [-opt]\n", argv[0]);
        return 1;
    }

    /* 
     * Open the input C file in read mode. 
     * Assign it to 'yyin' so the Flex lexer knows where to read from.
     */
    yyin = fopen(input_file, "r");
    if (!yyin) {
        fprintf(stderr, "Cannot open file: %s\n", input_file);
        return 1;
    }

    /* If the -tokens flag is set, print a header for the token stream */
    if (print_tokens) printf("TOKEN STREAM\n------------\n");

    /* 
     * Start the compilation process by calling the parser.
     * If yyparse() returns 0, it means the parsing was successful.
     */
    if (yyparse() == 0) {
        /* If parsing succeeded and -symtab was requested, print the table */
        if (print_symtab) print_symbol_table();
    }

    /* Close the opened input file to free resources */
    fclose(yyin);
    
    return 0; /* Return 0 to indicate successful execution */
}
