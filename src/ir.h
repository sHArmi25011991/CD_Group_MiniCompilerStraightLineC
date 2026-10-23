#ifndef IR_H
#define IR_H

typedef enum {
    IR_ASSIGN,
    IR_BINOP
} IRType;

typedef struct {
    IRType type;
    char   op;
    char   dest[64];
    char   arg1[64];
    char   arg2[64];
} IRInstr;

#define MAX_IR 1024

typedef struct {
    IRInstr items[MAX_IR];
    int     count;
} IRList;

#endif
