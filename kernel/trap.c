#include "riscv.h"

#include <kstdio.h>

#include <stdint.h>

// Handle CPU exceptions, including syscalls
__attribute__((interrupt)) void trap_handler(void) {
    // Exception cause is last 5 bits of mcause.
    uint32_t mcause = CSR_READ(mcause) & 0b11111;

    // Read syscall id from register a7
    uint32_t call = REG_READ(a7);

    // Exception code 8 is ecall from usermode
    if (mcause == 0x8) {

        // Switch on syscall
        if (call == 1) {
            // Read arguments, stored in a0 and a1
            char *b = (char*)REG_READ(a0);
            uint32_t n = REG_READ(a1);
            while (n--) {
                kputchar(*b++);
            }
        }

        // Advance mepc to return to next instruction in calling thread
        uint32_t mepc = CSR_READ(mepc);
        CSR_WRITE(mepc, mepc + 4);
        return;
    // Exception code 11 is ecall from machine mode
    } else if (mcause == 0xB) {
        kputs("Trap: Machine mode syscall registered.");
        // Advance mepc to return to next instruction in calling thread
        uint32_t mepc = CSR_READ(mepc);
        CSR_WRITE(mepc, mepc + 4);
        return;
    }

    // Any other exception code is a panic
    char *panic_template = "00";
    panic_template[1] = (mcause % 10) + '0';
    mcause /= 10;
    panic_template[0] = (mcause % 10) + '0';
    kputs("Trap: Panic occurred. Exception code: ");
    kputs(panic_template);

    // Loop forever (WDT will trigger a restart)
    while(1);
}
