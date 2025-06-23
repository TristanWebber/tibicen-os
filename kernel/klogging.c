#include "klogging.h"
#include "kstdio.h"
#include "riscv.h"
#include "task.h"

#include <stdint.h>

// Output contents of all registers
void klogging_regdump(trapframe_t *trapframe) {
    // Read all registers
    uint32_t sp = REG_READ(sp);
    uint32_t gp = REG_READ(gp);
    uint32_t tp = REG_READ(tp);
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
    kprintf("Occurred at task %i of %i.\r\n", current_task, task_count);
    kprintf("ra: %x sp: %x gp: %x tp: %x t0: %x\r\n", trapframe->ra, sp, gp, tp, trapframe->t0);
    kprintf("t1: %x t2: %x s0: %x s1: %x a0: %x\r\n", trapframe->t1, trapframe->t2, trapframe->s0, trapframe->s1, trapframe->a0);
    kprintf("a1: %x a2: %x a3: %x a4: %x a5: %x\r\n", trapframe->a1, trapframe->a2, trapframe->a3, trapframe->a4, trapframe->a5);
    kprintf("a6: %x a7: %x s2: %x s3: %x s4: %x\r\n", trapframe->a6, trapframe->a7, s2, s3, s4);
    kprintf("s5: %x s6: %x s7: %x s8: %x s9: %x\r\n", s5, s6, s7, s8, s9);
    kprintf("s10: %x s11: %x t3: %x t4: %x t5: %x\r\n", s10, s11, t3, t4, t5);
    kprintf("t6: %x\r\n", t6);
    kputs("");

    for (unsigned int task = 0; task < 4; task++) {
        task_context_t last_ctx = tasks[task].ctx;
        kprintf("Last saved context values for task %i:\r\n", task);
        kprintf("ra: %x sp: %x gp: %x tp: %x t0: %x\r\n", last_ctx.ra, last_ctx.sp, last_ctx.gp, last_ctx.tp, last_ctx.t0);
        kprintf("t1: %x t2: %x s0: %x s1: %x a0: %x\r\n", last_ctx.t1, last_ctx.t2, last_ctx.s0, last_ctx.s1, last_ctx.a0);
        kprintf("a1: %x a2: %x a3: %x a4: %x a5: %x\r\n", last_ctx.a1, last_ctx.a2, last_ctx.a3, last_ctx.a4, last_ctx.a5);
        kprintf("a6: %x a7: %x s2: %x s3: %x s4: %x\r\n", last_ctx.a6, last_ctx.a7, last_ctx.s2, last_ctx.s3, last_ctx.s4);
        kprintf("s5: %x s6: %x s7: %x s8: %x s9: %x\r\n", last_ctx.s5, last_ctx.s6, last_ctx.s7, last_ctx.s8, last_ctx.s9);
        kprintf("s10: %x s11: %x t3: %x t4: %x t5: %x\r\n", last_ctx.s10, last_ctx.s11, last_ctx.t3, last_ctx.t4, last_ctx.t5);
        kprintf("t6: %x\r\n", last_ctx.t6);
        kputs("");
    }

    kputs("Last saved context values for scheduler:");
    kprintf("ra: %x sp: %x gp: %x tp: %x t0: %x\r\n", ctx_scheduler.ra, ctx_scheduler.sp, ctx_scheduler.gp, ctx_scheduler.tp, ctx_scheduler.t0);
    kprintf("t1: %x t2: %x s0: %x s1: %x a0: %x\r\n", ctx_scheduler.t1, ctx_scheduler.t2, ctx_scheduler.s0, ctx_scheduler.s1, ctx_scheduler.a0);
    kprintf("a1: %x a2: %x a3: %x a4: %x a5: %x\r\n", ctx_scheduler.a1, ctx_scheduler.a2, ctx_scheduler.a3, ctx_scheduler.a4, ctx_scheduler.a5);
    kprintf("a6: %x a7: %x s2: %x s3: %x s4: %x\r\n", ctx_scheduler.a6, ctx_scheduler.a7, ctx_scheduler.s2, ctx_scheduler.s3, ctx_scheduler.s4);
    kprintf("s5: %x s6: %x s7: %x s8: %x s9: %x\r\n", ctx_scheduler.s5, ctx_scheduler.s6, ctx_scheduler.s7, ctx_scheduler.s8, ctx_scheduler.s9);
    kprintf("s10: %x s11: %x t3: %x t4: %x t5: %x\r\n", ctx_scheduler.s10, ctx_scheduler.s11, ctx_scheduler.t3, ctx_scheduler.t4, ctx_scheduler.t5);
    kprintf("t6: %x\r\n", ctx_scheduler.t6);
    kputs("");

    kputs("Last saved context values for os:");
    kprintf("ra: %x sp: %x gp: %x tp: %x t0: %x\r\n", ctx_os.ra, ctx_os.sp, ctx_os.gp, ctx_os.tp, ctx_os.t0);
    kprintf("t1: %x t2: %x s0: %x s1: %x a0: %x\r\n", ctx_os.t1, ctx_os.t2, ctx_os.s0, ctx_os.s1, ctx_os.a0);
    kprintf("a1: %x a2: %x a3: %x a4: %x a5: %x\r\n", ctx_os.a1, ctx_os.a2, ctx_os.a3, ctx_os.a4, ctx_os.a5);
    kprintf("a6: %x a7: %x s2: %x s3: %x s4: %x\r\n", ctx_os.a6, ctx_os.a7, ctx_os.s2, ctx_os.s3, ctx_os.s4);
    kprintf("s5: %x s6: %x s7: %x s8: %x s9: %x\r\n", ctx_os.s5, ctx_os.s6, ctx_os.s7, ctx_os.s8, ctx_os.s9);
    kprintf("s10: %x s11: %x t3: %x t4: %x t5: %x\r\n", ctx_os.s10, ctx_os.s11, ctx_os.t3, ctx_os.t4, ctx_os.t5);
    kprintf("t6: %x\r\n", ctx_os.t6);
    kputs("");

    for (unsigned int task = 0; task < 4; task++) {
        kprintf("Values in task %i stack:\r\n", task);
        for (unsigned int i = 0; i < 256; i++) {
            kprintf("0x%x ", task_stacks[task][i]);
            if (!(i + 1 % 20))
                kputs("");
        }

        kputs("");
    }

    for (unsigned int task = 0; task < 4; task++) {
        kprintf("Values in task %i kstack:\r\n", task);
        for (unsigned int i = 0; i < 256; i++) {
            kprintf("0x%x ", ppstack[task][i]);
            if (!(i + 1 % 20))
                kputs("");
        }

        kputs("");
    }

    kputs("Values in scheduler stack:");
    extern uint32_t _kstack_start[256];
    for (unsigned int i = 0; i < 256; i++) {
        kprintf("0x%x ", _kstack_start[i]);
        if (!(i + 1 % 20))
            kputs("");
    }
    kputs("");
}
