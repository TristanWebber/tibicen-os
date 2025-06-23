#ifndef KLOGGING_H
#define KLOGGING_H

#include "trap.h"

// Output contents of all registers
void klogging_regdump(trapframe_t *trapframe);

#endif /* KLOGGING_H */
