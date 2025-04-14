#include "task.h"
#include "timer.h"

#include <stdint.h>
#include <kstdio.h>

extern uint32_t _sbss, _ebss;

task_context_t ctx_os;
task_context_t ctx_task;
uint32_t task0_stack[1024];

static void main_clear_bss(void) {
    volatile uint32_t *this_word = &_sbss;
    while (this_word < &_ebss) {
        *this_word = 0;
        this_word++;
    }
}

int main(void) {

    main_clear_bss();
    init_wdt(1000);

    // Short delay ensures host terminal is ready
    int delay = 1000000;
    while (delay--) {}

    kputs("");
    kputs("Main: Hello from tibicen-os!");
    kputs("");

    kputs("Main: Switching to task0");

    // Configure context for user task and switch
    ctx_task.ra = (uint32_t)user_task0;
    ctx_task.sp = (uint32_t)&task0_stack[1023];
    _task_switch(&ctx_os, &ctx_task);

    kputs("Main: Control has been returned to the kernel.");

    kputs("");
    kputs("Main: Goodbye from tibicen-os!");
    kputs("");

    while(1) {
        feed_wdt();
    }

    return 0;
}
