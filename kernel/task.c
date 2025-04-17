#include "task.h"
#include "timer.h"

#include <stdbool.h>
#include <stdint.h>

#define MAX_TASK_COUNT 4
#define TASK_STACK_WORDS 1024

uint32_t task_count = 0;
uint32_t current_task = 0;

// Store os state when handed to scheduler
task_context_t ctx_os;

// User created tasks (future: linked list)
task_handle_t tasks[MAX_TASK_COUNT];

// Array for task stacks (future: heap allocated)
uint32_t task_stacks[MAX_TASK_COUNT][TASK_STACK_WORDS];

// Spawn a task to run
bool task_create(void *task) {
    if (task_count == MAX_TASK_COUNT) {
        return false;
    }

    task_context_t new_task = {0};
    new_task.ra = (uint32_t)task;
    new_task.sp = (uint32_t)&task_stacks[task_count][TASK_STACK_WORDS - 1];

    tasks[task_count].ctx = new_task;
    tasks[task_count].task_function = task;
    tasks[task_count].state = READY;

    task_count++;
    return true;
}

// Hand control to the scheduler
void tasks_run() {
    tasks[0].state = RUNNING;
    _task_switch(&ctx_os, &tasks[0].ctx);
}

// Remove a task from the scheduler
void task_delete() {
    tasks[current_task].state = DELETED;
    task_yield();
}

// Round-robin scheduler
void task_yield() {

    feed_wdt();

    // Yielding task is moved to READY, or held in DELETED
    uint32_t last_task = current_task;
    tasks[last_task].state = (tasks[last_task].state == RUNNING) ? READY : DELETED;

    // Round-robin scheduler - check for any tasks in READY state
    for (uint32_t i = 0; i < MAX_TASK_COUNT; i++) {
        current_task = (current_task + 1) % MAX_TASK_COUNT;
        if (tasks[current_task].state == READY) {
            tasks[current_task].state = RUNNING;
            _task_switch(&tasks[last_task].ctx, &tasks[current_task].ctx);
            return;
        }
    }

    // If we fell through, it's time to hand back to the os
    _task_switch(&tasks[last_task].ctx, &ctx_os);
}
