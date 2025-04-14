#ifndef RISCV_H
#define RISCV_H

// Macros for normal register and DMA operations
#define BIT(x) ((uint32_t) 1U << (x))
#define REG_RW(base, offset) (*(volatile uint32_t *) ((base) + (offset)))

// Read the value of a normal cpu register
#define REG_READ(reg) ({                     \
    uint32_t v_;                             \
    asm volatile("mv %0, " #reg : "=r"(v_)); \
    v_;                                      \
})

// Macros for CSR register operations
#define CSR_WRITE(reg, val) ({ asm volatile("csrw " #reg ", %0" ::"rK"(val)); })
#define CSR_READ(reg) ({                       \
    uint32_t v_;                               \
    asm volatile("csrr %0, " #reg : "=r"(v_)); \
    v_;                                        \
})
#define CSR_SETBITS(reg, cm, sm) CSR_WRITE(reg, (CSR_READ(reg) & ~(cm)) | (sm))

#endif /* RISCV_H */
