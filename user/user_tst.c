#include "kstdio.h"

__attribute__((section(".user_text"))) void user_func(void) {
    kputs("User: Entered user mode.");
    asm volatile ("ecall");
}
