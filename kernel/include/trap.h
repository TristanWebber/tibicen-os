#ifndef TRAP_H
#define TRAP_H

#include <stdint.h>

// Caller saved registers - used in ISRs
typedef struct trapframe {
    uint32_t ra;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t s0;
    uint32_t s1;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t a4;
    uint32_t a5;
    uint32_t a6;
    uint32_t a7;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
} trapframe_t;

// Possible mcause values
typedef enum mcause {
    MCAUSE_PMP_FAULT = 0x1,
    MCAUSE_ILLEGAL_INSTRUCTION = 0x2,
    MCAUSE_EBREAK = 0x3,
    MCAUSE_PMP_LOAD = 0x5,
    MCAUSE_PMP_READ = 0x7,
    MCAUSE_ECALL_USER = 0x8,
    MCAUSE_ECALL_MACHINE = 0xB,
} mcause_t;

// Available syscall args
typedef enum syscall {
    SYS_WRITE = 1,
    SYS_TASK_CREATE,
    SYS_TASK_DELETE,
    SYS_TASK_YIELD,
    SYS_TASK_DELAY,
} syscall_t;

#endif /* TRAP_H */
