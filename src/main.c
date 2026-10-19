#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
extern int   yyparse(void);

int print_tokens = 0;
int lineno       = 1;

int main(int argc, char **argv) {
    const char *input_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-tokens") == 0)
            print_tokens = 1;
        else
            input_file = argv[i];
    }

    if (!input_file) {
        fprintf(stderr, "Usage: %s <input.c> [-tokens]\n", argv[0]);
        return 1;
    }

    yyin = fopen(input_file, "r");
    if (!yyin) {
        fprintf(stderr, "Cannot open file: %s\n", input_file);
        return 1;
    }

    if (print_tokens) printf("TOKEN STREAM\n------------\n");

    int rc = yyparse();

    fclose(yyin);
    return rc;
}
