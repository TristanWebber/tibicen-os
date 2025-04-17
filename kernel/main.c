#include "task.h"
#include "timer.h"

#include <stdint.h>
#include <kstdio.h>

extern uint32_t _sbss, _ebss;

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

    // Spawn user tasks
    tasks_init();

    kputs("Main: Switching to task scheduler");

    // Hand control to the scheduler thread
    tasks_run();

    kputs("Main: Control has been returned to the kernel.");

    kputs("");
    kputs("Main: Goodbye from tibicen-os!");
    kputs("");

    while(1) {
        feed_wdt();
    }

    return 0;
}
