#include "riscv.h"
#include "task.h"
#include "timer.h"

#include <kstdio.h>

#include <stdint.h>

extern uint32_t _sbss, _ebss, _sdata;

static void main_clear_bss(void) {
    volatile uint32_t *this_word = &_sbss;
    while (this_word < &_ebss) {
        *this_word = 0;
        this_word++;
    }
}

// ecall from user mode
void user_func(void);

// Initialise Physical Memory Protection to allow user access to entire address
// space
void pmp_init(void);

int main(void) {

    main_clear_bss();
    timer_init_wdt(1000);

    // Short delay ensures host terminal is ready
    int delay = 1000000;
    while (delay--) {}

    kputs("");
    kputs("Main: Hello from tibicen-os!");
    kputs("");

    // Spawn user tasks
    tasks_init();

    kputs("Main: Switching to task scheduler");

    // Hand control to the scheduler thread
    tasks_run();

    kputs("Main: Task scheduler has finished.");

    kputs("");
    kputs("Main: Testing syscalls...");

    // ecall from machine mode
    asm volatile ("ecall");

    // Allow user mode to access memory space
    pmp_init();

    // Set user mode
    uint32_t mstatus = CSR_READ(mstatus);
    mstatus &= ~(1 << 11);
    CSR_WRITE(mstatus, mstatus);

    // Set mepc to user_func
    CSR_WRITE(mepc, user_func);
    // Set return address manually and jump to a function in usermode
    asm volatile ("auipc t0, 0");
    asm volatile ("addi t0, t0, 12");
    asm volatile ("mv ra, t0");
    asm volatile ("mret");

    kputs("");
    kputs("Main: Goodbye from tibicen-os!");
    kputs("");

    while(1) {
        timer_feed_wdt();
    }

    return 0;
}

// Initialise Physical Memory Protection to allow user access to entire address
// space
void pmp_init(void) {
    // Allow user to access all memory using TOR method
    CSR_WRITE(pmpaddr0, (uint32_t)&_sdata >> 2); // dram start
    CSR_WRITE(pmpaddr1, 0x600D0FFF >> 2); // dma end
    CSR_WRITE(pmpaddr2, 0xFFFFFFFF >> 2); // memory end
    uint32_t pmpcfg0 = 0x08;              // No access below dram
    pmpcfg0 |= (0x0F << 8);               // Full access dram to dma
    pmpcfg0 |= (0x08 << 16);              // No access beyond dma
    CSR_WRITE(pmpcfg0, pmpcfg0);
}
