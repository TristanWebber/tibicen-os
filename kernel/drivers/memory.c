#include "memory.h"
#include "riscv.h"

void mem_clear_bss(void) {
    // Clear kernel bss
    volatile uint32_t *this_word = &_sbss;
    while (this_word < &_ebss) {
        *this_word = 0;
        this_word++;
    }

    // Clear user bss
    this_word = &_subss;
    while (this_word < &_eubss) {
        *this_word = 0;
        this_word++;
    }
}

// Initialise Physical Memory Protection to prevent user accessing any address
// outside userspace
static void pmp_init(void) {
    // Using TOR method to set ranges to apply protection settings to
    // pmpaddr0 through pmpaddr3 are controlled by pmpcfg0
    CSR_WRITE(pmpaddr0, (uint32_t)&_sudata >> 2);// user dram start
    CSR_WRITE(pmpaddr1, (uint32_t)&_eudram >> 2);// user dram end
    CSR_WRITE(pmpaddr2, (uint32_t)&_eiram >> 2); // kernel iram end
    CSR_WRITE(pmpaddr3, (uint32_t)&_euiram >> 2);// user iram end

    // pmpaddr4 through pmpaddr7 are controlled by pmpcfg1
    CSR_WRITE(pmpaddr4, MEMORY_END >> 2);        // addressable range end

    // pmpcfg registers each contain 4 bytes encoding how to treat addresses
    // described in pmpaddrx
    uint32_t pmpcfg0 = TOR_PROTECTED;            // No access below user dram
    pmpcfg0 |= (TOR_RWX << 8);                   // RWX access to user dram
    pmpcfg0 |= (TOR_PROTECTED << 16);            // No access to kernel iram
    pmpcfg0 |= (TOR_RWX << 24);                  // RWX access to user iram
    CSR_WRITE(pmpcfg0, pmpcfg0);

    uint32_t pmpcfg1 = TOR_PROTECTED;            // No access beyond user iram
    CSR_WRITE(pmpcfg1, pmpcfg1);
}

void mem_init(void) {
    pmp_init();
}
