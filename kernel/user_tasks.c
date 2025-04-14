#include "riscv.h"
#include "task.h"

#include <kstdio.h>

void user_task0(void) {
    // Brittle, but works for POC. The os and task context were passed to
    // _task_switch in registers a0 and a1
    task_context_t *ctx_os = (task_context_t*)REG_READ(a0);
    task_context_t *ctx_task = (task_context_t*)REG_READ(a1);

    kputs("Task0: Context Switch Success !");
    kputs("Task0: Attempt to pass control back to os...");

    _task_switch(ctx_task, ctx_os);
}
