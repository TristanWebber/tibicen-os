#include "task.h"

#include <kstdio.h>
#include <stdint.h>

void user_task0(void) {
    for (uint32_t i = 0; i < 4; i++) {
        kputs("Task0: Context Switch Success !");
        kputs("Task0: Attempt to pass control back to os...");
        task_yield();
    }
    task_delete();
}

void user_task1(void) {
    for (uint32_t i = 0; i < 3; i++) {
        kputs("Task1: Context Switch Success !");
        kputs("Task1: Attempt to pass control back to os...");
        task_yield();
    }
    task_delete();
}

void user_task2(void) {
    kputs("Task2: Context Switch Success !");
    kputs("Task2: Attempt to pass control back to os...");
    task_delete();
}

void user_task3(void) {
    for (uint32_t i = 0; i < 5; i++) {
        kputs("Task3: Context Switch Success !");
        kputs("Task3: Attempt to pass control back to os...");
        task_yield();
    }
    task_delete();
}

void tasks_init(void) {
    task_create(user_task0);
    task_create(user_task1);
    task_create(user_task2);
    task_create(user_task3);
}
