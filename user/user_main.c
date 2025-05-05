// Demonstration user application with syscalls
#include "stdio.h"
#include "user.h"

#include <stdint.h>

#define SECONDS_TO_MICROS(time_in_seconds) (time_in_seconds * 1000000)
#define MILLIS_TO_MICROS(time_in_ms) (time_in_ms * 1000)

// Repeated task
void user_task1(void) {
    for (uint32_t i = 0; i < 4; i++) {
        puts("user_task1: Context Switch Success !");
        sys_task_yield();
    }
    puts("user_task1: Finished.");
    sys_task_delete();
}

// One-off task
void user_task2(void) {
    puts("user_task2: Context Switch Success !");
    puts("user_task2: Finished.");
    sys_task_delete();
}

// Sleeping task
void user_task3(void) {
    for (uint32_t i = 0; i < 5; i++) {
        puts("user_task3: Context Switch Success !");
        sys_task_delay(SECONDS_TO_MICROS(1));
    }
    puts("user_task3: Finished.");
    sys_task_delete();
}

// Infinite loop main
void user_main(void) {
    puts("user_main: Starting user_main !");
    sys_task_create(user_task1);
    sys_task_create(user_task2);
    sys_task_create(user_task3);
    while (1) {
        puts("user_main: Sleeping for 250ms.");
        sys_task_delay(MILLIS_TO_MICROS(250));
    }
    sys_task_delete();
}
