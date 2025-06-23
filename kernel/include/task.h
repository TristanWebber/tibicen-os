#ifndef TASK_H
#define TASK_H

#include <stdbool.h>
#include <stdint.h>

extern uint32_t task_count;
extern uint32_t current_task;

// Array for user task stacks (future: heap allocated)
extern uint32_t task_stacks[4][256];

// Array for per-process kernel stacks
extern uint32_t ppstack[4][256];

// Callee saved registers - used in task switching
typedef struct task_context {
    uint32_t ra;
    uint32_t sp;

    // callee-saved
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t s8;
    uint32_t s9;
    uint32_t s10;
    uint32_t s11;

    // caller-saved
    uint32_t gp;
    uint32_t tp;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
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
    uint32_t mepc;
} task_context_t;

// First entry needs to be `DELETED` state so unallocated tasks do not attempt to run
typedef enum task_state {
    DELETED,
    NEW,
    READY,
    PENDING,
    RUNNING,
} task_state_t;

typedef struct task_handle {    // offset in bytes (for use in asm)
    // Stack pointers at start is easy for interrupt handlers
    uint32_t kernel_sp;         // 0
    uint32_t user_sp;           // 4

    void *task_function;        // 8
    task_state_t state;         // 12
    uint64_t delay_to_us;       // 16 (2 words)
    task_context_t ctx;         // 24
} task_handle_t;

extern task_handle_t tasks[4];

extern task_context_t ctx_os;
extern task_context_t ctx_scheduler;

extern void _task_switch(task_context_t *ctx_old, task_context_t *ctx_new, int to_user_mode, task_handle_t *next_task);

extern void _task_store(task_context_t *ctx);

bool task_create(void *task_function);

void tasks_run(void);

void task_delete(void);

void task_yield(void);

void task_delay_us(uint64_t delay_us);

void task_clear_stacks(void);

#endif /* TASK_H */
