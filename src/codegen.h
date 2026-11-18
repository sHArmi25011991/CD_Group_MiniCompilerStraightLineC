#ifndef CODEGEN_H
#define CODEGEN_H

#include "ir.h"

void print_ir(const IRList *ir);
void generate_assembly(const IRList *ir);

/* Register descriptor support */
extern int emit_descriptor;
void print_reg_descriptor(const char *reg_val);

#endif
