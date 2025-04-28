// Proof of concept for userspace executing a syscall
#include "stdio.h"

void user_func(void) {
    puts("User: Entered user mode.");
}
