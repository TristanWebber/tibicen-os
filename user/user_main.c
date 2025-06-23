// Demonstration user application with syscalls
#include "stdio.h"
#include "user.h"

#include <stdint.h>

#define SECONDS_TO_MICROS(time_in_seconds) (time_in_seconds * 1000000)
#define MILLIS_TO_MICROS(time_in_ms) (time_in_ms * 1000)

// Repeated task
void user_task1(void) {
    for (uint32_t i = 0; i < 4; i++) {
        printf("user_task1: Context Switch Success ! Iteration: %i\r\n", i);
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
        printf("user_task3: Context Switch Success ! Iteration: %i\r\n", i);
        sys_task_delay(SECONDS_TO_MICROS(1));
    }
    puts("user_task3: Finished.");
    sys_task_delete();
}

// New task -> Create after another task has ended
void user_task4(void) {
    puts("user_task4: Context Switch Success !");
    puts("user_task4: Finished.");
    sys_task_delete();
}

// Helper to output diagnostics on task creation
void task_checker(bool res) {
    if (res) {
        puts("Task creation success.");
    } else {
        puts("Task creation failure.");
    }
}

// Test and demonstrate prinf functionality
void test_printf(void) {
    printf("Checking printf escape: %% \r\n");
    printf("Checking printf for chars: %c \r\n", 'c');
    printf("Checking printf for strings: %s \r\n", "string");
    printf("Checking printf for integer: %d \r\n", -69);
    printf("Checking printf for unsigned integer: %u \r\n", 69);
    printf("Checking printf for hex integer: 0x%x \r\n", 1077411840);
    printf("Checking printf for octal integer: 0o%o \r\n", 12345678);
    char *ptr = "pointer";
    printf("Checking printf for pointer: 0x%p \r\n", ptr);

    printf("Checking printf for error: %ld \r\n", 69);
}

// Infinite loop main
void user_main(void) {
    puts("user_main: Starting user_main !");
    sys_task_create(user_task1);
    sys_task_create(user_task2);
    sys_task_create(user_task3);

    // Yield and delay to allow some tasks to finish
    sys_task_delay(SECONDS_TO_MICROS(5));

    // Task 1 and 2 should now be complete -> Attempt to create new tasks
    task_checker(sys_task_create(user_task4));
    task_checker(sys_task_create(user_task4));

    // Task array will be full - This should fail
    task_checker(sys_task_create(user_task4));

    test_printf();

    while (1) {
        puts("user_main: Sleeping for 250ms.");
        sys_task_delay(MILLIS_TO_MICROS(250));
    }
    sys_task_delete();
}
