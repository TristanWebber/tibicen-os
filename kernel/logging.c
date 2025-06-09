#include "logging.h"
#include "kstdio.h"
#include "riscv.h"

#include <stdint.h>

// Output contents of all registers
void kregdump(void) {
    // Read all registers
    uint32_t ra = REG_READ(ra);
    uint32_t sp = REG_READ(sp);
    uint32_t gp = REG_READ(gp);
    uint32_t tp = REG_READ(tp);
    uint32_t t0 = REG_READ(t0);
    uint32_t t1 = REG_READ(t1);
    uint32_t t2 = REG_READ(t2);
    uint32_t s0 = REG_READ(s0);
    uint32_t s1 = REG_READ(s1);
    uint32_t a0 = REG_READ(a0);
    uint32_t a1 = REG_READ(a1);
    uint32_t a2 = REG_READ(a2);
    uint32_t a3 = REG_READ(a3);
    uint32_t a4 = REG_READ(a4);
    uint32_t a5 = REG_READ(a5);
    uint32_t a6 = REG_READ(a6);
    uint32_t a7 = REG_READ(a7);
    uint32_t s2 = REG_READ(s2);
    uint32_t s3 = REG_READ(s3);
    uint32_t s4 = REG_READ(s4);
    uint32_t s5 = REG_READ(s5);
    uint32_t s6 = REG_READ(s6);
    uint32_t s7 = REG_READ(s7);
    uint32_t s8 = REG_READ(s8);
    uint32_t s9 = REG_READ(s9);
    uint32_t s10 = REG_READ(s10);
    uint32_t s11 = REG_READ(s11);
    uint32_t t3 = REG_READ(t3);
    uint32_t t4 = REG_READ(t4);
    uint32_t t5 = REG_READ(t5);
    uint32_t t6 = REG_READ(t6);

    // Print contents of all registers
    kputs("Core dump:");
    kprintf("ra: %x sp: %x gp: %x tp: %x t0: %x\r\n", ra, sp, gp, tp, t0);
    kprintf("t1: %x t2: %x s0: %x s1: %x a0: %x\r\n", t1, t2, s0, s1, a0);
    kprintf("a1: %x a2: %x a3: %x a4: %x a5: %x\r\n", a1, a2, a3, a4, a5);
    kprintf("a6: %x a7: %x s2: %x s3: %x s4: %x\r\n", a6, a7, s2, s3, s4);
    kprintf("s5: %x s6: %x s7: %x s8: %x s9: %x\r\n", s5, s6, s7, s8, s9);
    kprintf("s10: %x s11: %x t3: %x t4: %x t5: %x\r\n", s10, s11, t3, t4, t5);
    kprintf("t6: %x\r\n", t6);
}
