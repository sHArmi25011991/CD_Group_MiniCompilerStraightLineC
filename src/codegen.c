#include <stdio.h>
#include <string.h>
#include "codegen.h"

#define NREGS 4

static char reg_desc[NREGS][64];
static int  reg_next = 0;

static int alloc_reg(void) {
    if (reg_next >= NREGS) return -1;
    return reg_next++;
}

static int find_in_reg(const char *name) {
    for (int i = 0; i < NREGS; i++)
        if (strcmp(reg_desc[i], name) == 0) return i;
    return -1;
}

static void put_in_reg(int r, const char *name) {
    strncpy(reg_desc[r], name, sizeof(reg_desc[r]) - 1);
}

static void clear_reg(int r) { reg_desc[r][0] = '\0'; }

void print_ir(const IRList *ir) {
    for (int i = 0; i < ir->count; i++) {
        const IRInstr *ins = &ir->items[i];
        if (ins->type == IR_ASSIGN)
            printf("%s = %s\n", ins->dest, ins->arg1);
        else
            printf("%s = %s %c %s\n",
                   ins->dest, ins->arg1, ins->op, ins->arg2);
    }
}

void generate_assembly(const IRList *ir) {
    for (int i = 0; i < NREGS; i++) clear_reg(i);
    reg_next = 0;

    for (int i = 0; i < ir->count; i++) {
        const IRInstr *ins = &ir->items[i];

        if (ins->type == IR_ASSIGN) {
            int r1 = find_in_reg(ins->arg1);
            if (r1 < 0) {
                r1 = alloc_reg();
                if (r1 < 0) r1 = 0;
                printf("MOV R%d, %s\n", r1, ins->arg1);
                put_in_reg(r1, ins->arg1);
            }
            printf("MOV %s, R%d\n", ins->dest, r1);
            put_in_reg(r1, ins->dest);
        } else {
            int r1 = find_in_reg(ins->arg1);
            if (r1 < 0) {
                r1 = alloc_reg();
                if (r1 < 0) r1 = 0;
                printf("MOV R%d, %s\n", r1, ins->arg1);
                put_in_reg(r1, ins->arg1);
            }
            int r2 = find_in_reg(ins->arg2);
            if (r2 < 0) {
                r2 = alloc_reg();
                if (r2 < 0) r2 = NREGS - 1;
                printf("MOV R%d, %s\n", r2, ins->arg2);
                put_in_reg(r2, ins->arg2);
            }
            const char *mnemonic =
                ins->op == '+' ? "ADD" :
                ins->op == '-' ? "SUB" :
                ins->op == '*' ? "MUL" : "DIV";
            printf("%s R%d, R%d\n", mnemonic, r1, r2);
            clear_reg(r2);
            put_in_reg(r1, ins->dest);
        }
    }
}
