#include <string.h>
#include "opt.h"

void optimize_ir(IRList *ir) {
    for (int i = 0; i < ir->count; i++) {
        IRInstr *ins = &ir->items[i];
        int used_later = 0;
        
        for (int j = i + 1; j < ir->count; j++) {
            IRInstr *nx = &ir->items[j];
            if (strcmp(nx->arg1, ins->dest) == 0 ||
                strcmp(nx->arg2, ins->dest) == 0) {
                used_later = 1;
                break;
            }
        }
        
        if (!used_later && ins->type == IR_ASSIGN) {
            strcpy(ins->dest, "dead"); 
        }
    }
}
