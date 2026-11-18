#include <stdio.h>
#include <stdlib.h>    /* Required for atoi() */
#include <string.h>
#include "codegen.h"

#define NREGS 4
#define MAX_TEMPS 100

static char reg_desc[NREGS][64];
static int  reg_next = 0;

/* Store the evaluated expression for each temporary (e.g., t1 -> c*d) */
static char temp_map[MAX_TEMPS][64];

/* 
 * NOTE: `emit_descriptor` is DEFINED in main.c. 
 * We must NOT define it here to avoid multiple definition errors.
 */

static int alloc_reg(void) {
    if (reg_next >= NREGS) return -1;
    return reg_next++;
}

/* Find which register currently holds the given variable/value */
static int find_in_reg(const char *name) {
    for (int i = 0; i < NREGS; i++)
        if (strcmp(reg_desc[i], name) == 0) return i;
    return -1;
}

/* Update the register descriptor with a new value */
static void put_in_reg(int r, const char *name) {
    strncpy(reg_desc[r], name, sizeof(reg_desc[r]) - 1);
}

/* Clear a register's descriptor */
static void clear_reg(int r) { reg_desc[r][0] = '\0'; }

/* 
 * Print the current state of all registers (Register Descriptor).
 * The `reg_val` parameter allows us to show the evaluated expression (e.g., c*d) 
 * instead of the raw temporary (e.g., t1) for better readability.
 */
void print_reg_descriptor(const char *reg_val) {
    if (!emit_descriptor) return;
    printf("  [Register Descriptor] ");
    for (int i = 0; i < NREGS; i++) {
        if (reg_desc[i][0] == '\0')
            printf("R%d=(empty) ", i);
        else if (strcmp(reg_desc[i], reg_val) == 0)
            printf("R%d=%s ", i, reg_val); 
        else
            printf("R%d=%s ", i, reg_desc[i]);
    }
    printf("\n");
}

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

/* Helper function to resolve a name. If it's a temporary like 't1', return its mapped expression (e.g., c*d) */
static const char* resolve_name(const char *name) {
    if (name[0] == 't' && name[1] >= '0' && name[1] <= '9') {
        int idx = atoi(name + 1);
        if (idx < MAX_TEMPS && temp_map[idx][0] != '\0')
            return temp_map[idx];
    }
    return name;
}

void generate_assembly(const IRList *ir) {
    for (int i = 0; i < NREGS; i++) clear_reg(i);
    for (int i = 0; i < MAX_TEMPS; i++) temp_map[i][0] = '\0';
    reg_next = 0;

    for (int i = 0; i < ir->count; i++) {
        const IRInstr *ins = &ir->items[i];
        int is_dead = (strcmp(ins->dest, "dead") == 0);

        if (ins->type == IR_ASSIGN) {
            int r1 = find_in_reg(ins->arg1);
            if (r1 < 0) {
                r1 = alloc_reg();
                if (r1 < 0) r1 = 0;
                printf("MOV R%d, %s\n", r1, ins->arg1);
                put_in_reg(r1, ins->arg1);
                print_reg_descriptor(ins->arg1);
            }
            if (!is_dead) {
                printf("MOV %s, R%d\n", ins->dest, r1);
                put_in_reg(r1, ins->dest);
                print_reg_descriptor(ins->dest);
            }
        } else {
            /* Resolve the real names (e.g., if arg1 is t1, it becomes c*d) */
            const char *a1 = resolve_name(ins->arg1);
            const char *a2 = resolve_name(ins->arg2);

            int r1 = find_in_reg(a1);
            if (r1 < 0) {
                r1 = alloc_reg();
                if (r1 < 0) r1 = 0;
                printf("MOV R%d, %s\n", r1, a1);
                put_in_reg(r1, a1);
                print_reg_descriptor(a1);
            }
            int r2 = find_in_reg(a2);
            if (r2 < 0) {
                r2 = alloc_reg();
                if (r2 < 0) r2 = NREGS - 1;
                printf("MOV R%d, %s\n", r2, a2);
                put_in_reg(r2, a2);
                print_reg_descriptor(a2);
            }
            const char *mnemonic =
                ins->op == '+' ? "ADD" :
                ins->op == '-' ? "SUB" :
                ins->op == '*' ? "MUL" : "DIV";

            if (!is_dead) {
                printf("%s R%d, R%d\n", mnemonic, r1, r2);
                
                /* Construct the evaluated expression (e.g., b+c*d) */
                char expr[128];
                snprintf(expr, sizeof(expr), "%s%c%s", a1, ins->op, a2);

                /* If the destination is a temporary (e.g., t2), remember what it holds (b+c*d) */
                if (ins->dest[0] == 't' && ins->dest[1] >= '0' && ins->dest[1] <= '9') {
                    int idx = atoi(ins->dest + 1);
                    if (idx < MAX_TEMPS) {
                        strncpy(temp_map[idx], expr, 63);
                    }
                }

                clear_reg(r2);
                put_in_reg(r1, expr); /* Store the expression in the descriptor */
                print_reg_descriptor(expr);
            } else {
                clear_reg(r2);
            }
        }
    }
}
