#ifndef TASK_H
#define TASK_H

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

extern void _task_switch(task_context_t *ctx_old, task_context_t *ctx_new);

void user_task0(void);

#endif /* TASK_H */
