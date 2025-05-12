#include "riscv.h"
#include "task.h"
#include "trap.h"

#include <kstdio.h>

#include <stdint.h>

// Handle CPU exceptions, including syscalls
void* trap_handler(trapframe_t *trapframe) {
    // Exception cause is last 5 bits of mcause.
    mcause_t mcause = CSR_READ(mcause) & 0b11111;

    // Advance mepc to return to next instruction in calling thread
    uint32_t mepc = CSR_READ(mepc);
    CSR_WRITE(mepc, mepc + 4);

    // Some syscalls have a return value
    void *res = 0;

    // Exception code 8 is ecall from usermode
    if (mcause == MCAUSE_ECALL_USER) {

        // Switch on syscall
        switch(trapframe->a7) {
            case SYS_WRITE: {
                // Read arguments, stored in a0 and a1
                char *b = (char *)trapframe->a0;
                uint32_t n = trapframe->a1;
                while (n--) {
                    res += kputchar(*b++);
                }
                break;
            }
            case SYS_TASK_CREATE: {
                // Function pointer in a0
                void *fn = (void *)trapframe->a0;
                res = (void *)task_create(fn);
                break;
            }
            case SYS_TASK_DELETE:
                task_delete();
                break;
            case SYS_TASK_YIELD:
                task_yield();
                break;
            case SYS_TASK_DELAY: {
                // Read argument, stored in a0 and a1
                uint64_t us = ((uint64_t)trapframe->a1 << 32) | trapframe->a0;
                task_delay_us(us);
                break;
            }
            default:
                kputs("Unknown syscall registered.");
                break;
        }

        // Returns to asm routine
        return res;

    // Exception code 11 is ecall from machine mode
    } else if (mcause == MCAUSE_ECALL_MACHINE) {
        kputs("Trap: Machine mode syscall registered.");
        // Nothing implemented: Loop forever (WDT will trigger a restart)
        while(true);
    }

    // Any other exception code is a panic
    kprintf("Trap: Panic occurred. Exception code: %u\r\n", mcause);
    kprintf("Trap: Occurred at instruction: 0x%x\r\n", mepc);

    // Nothing implemented: Loop forever (WDT will trigger a restart)
    while(1);
}
