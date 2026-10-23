#ifndef CODEGEN_H
#define CODEGEN_H

#include "ir.h"

void print_ir(const IRList *ir);
void generate_assembly(const IRList *ir);

#endif
