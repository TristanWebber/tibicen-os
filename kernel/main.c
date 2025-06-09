#include "memory.h"
#include "task.h"
#include "timer.h"

#include <kstdio.h>

#include <stdint.h>

// Forward declaration of user's main function
void user_main(void);

int main(void) {

    mem_clear_bss();
    timer_init_wdt(1000);

    // Short delay ensures host terminal is ready
    int delay = 1000000;
    while (delay--) {}

    kputs("");
    kputs("Main: Hello from tibicen-os!");
    kputs("");

    // Configure memory protection
    mem_init();

    // Create user_main task
    task_create(user_main);

    // Start the systick timer
    timer_systick_init();

    // Start task scheduler
    tasks_run();

    kputs("");
    kputs("Main: Goodbye from tibicen-os!");
    kputs("");

    while(1) {
        timer_feed_wdt();
    }

    return 0;
}
