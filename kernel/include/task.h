#ifndef TASK_H
#define TASK_H

#include <stdbool.h>
#include <stdint.h>

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
} task_context_t;

// DELETED needs to be first entry so unallocated tasks do not attempt to run
typedef enum task_state {
    DELETED,
    READY,
    RUNNING,
} task_state_t;

typedef struct task_handle {
    void *task_function;
    task_context_t ctx;
    task_state_t state;
} task_handle_t;

extern void _task_switch(task_context_t *ctx_old, task_context_t *ctx_new);

bool task_create(void *task_function);

// User defined function to spawn tasks
void tasks_init(void);

void tasks_run(void);

void task_delete(void);

void task_yield(void);

#endif /* TASK_H */
