#include "task.h"
#include "riscv.h"
#include "timer.h"

#include <kstdio.h>

#include <stdbool.h>
#include <stdint.h>

#define MAX_TASK_COUNT 4
#define TASK_STACK_WORDS 1024

uint32_t task_count = 0;
uint32_t current_task = 0;

// Store os and scheduler states
task_context_t ctx_os;
task_context_t ctx_scheduler;

// User created tasks (future: linked list)
task_handle_t tasks[MAX_TASK_COUNT];

// Array for task stacks (future: heap allocated)
extern uint32_t task_stacks[MAX_TASK_COUNT][TASK_STACK_WORDS];

// Spawn a task to run
bool task_create(void *task) {
    if (task_count == MAX_TASK_COUNT) {
        return false;
    }

    // Find a free slot.
    uint32_t free_idx = UINT32_MAX;
    for (uint32_t i = 0; i < MAX_TASK_COUNT; i++) {
        if (tasks[i].state == DELETED) {
            free_idx = i;
            break;
        }
    }

    // Test a free slot was found
    if (free_idx == UINT32_MAX) {
        return false;
    }

    task_context_t new_task = {0};
    new_task.ra = (uint32_t)task;
    new_task.sp = (uint32_t)&task_stacks[free_idx][TASK_STACK_WORDS - 1];

    tasks[free_idx].ctx = new_task;
    tasks[free_idx].task_function = task;
    tasks[free_idx].state = NEW;
    tasks[free_idx].delay_to_us = 0;

    task_count++;
    return true;
}

void tasks_scheduler(void) {

    // Fire the first task if it has been initialised
    kputs("Tasks: Starting the tasks scheduler.");
    _task_switch(&ctx_scheduler, &tasks[0].ctx, 2);

    while(task_count) {
        timer_feed_wdt();
        current_task = (current_task + 1) % MAX_TASK_COUNT;
        task_state_t current_task_state = tasks[current_task].state;

        switch(current_task_state) {
            case NEW:
                tasks[current_task].state = RUNNING;
                _task_switch(&ctx_scheduler, &tasks[current_task].ctx, 2);
                break;
            case READY:
                tasks[current_task].state = RUNNING;
                _task_switch(&ctx_scheduler, &tasks[current_task].ctx, 0);
                break;
            case PENDING:
                // Switch tasks where timer has expired
                if (timer_uptime_us() >= tasks[current_task].delay_to_us) {
                    tasks[current_task].state = RUNNING;
                    tasks[current_task].delay_to_us = 0;
                    _task_switch(&ctx_scheduler, &tasks[current_task].ctx, 0);
                }
                break;
            case DELETED:
                break;
            case RUNNING:
            default:
                kputs("Tasks: WARNING - Invalid state in scheduler.");
                break;
        }
    }

    // If we fell through, it's time to hand back to the os
    _task_switch(&ctx_scheduler, &ctx_os, 0);
}

// Hand control to the scheduler
void tasks_run() {

    // Store os original context
    _task_store(&ctx_os);
    if (!task_count) {
        kputs("Tasks: No tasks to run.");
        return;
    }

    // Only call the scheduler if at least one task has been created
    if (tasks[0].ctx.ra != 0) {
        tasks[0].state = RUNNING;

        // Set user mode - scheduler will move to userspace
        uint32_t mstatus = CSR_READ(mstatus);
        mstatus &= ~(1 << 11);
        CSR_WRITE(mstatus, mstatus);

        tasks_scheduler();
    }
}

// Remove currently running task from the scheduler
void task_delete() {
    tasks[current_task].state = DELETED;
    task_count--;
    task_yield();
}

// Place a task in PENDING for at least delay_us
void task_delay_us(uint64_t delay_us) {
    tasks[current_task].state = PENDING;

    // Convert delay_us to delay_to_us
    uint64_t time_us_now = timer_uptime_us();
    tasks[current_task].delay_to_us = time_us_now + delay_us;

    task_yield();
}

// Round-robin scheduler
void task_yield() {

    // Yielding task is moved to READY if RUNNING. Otherwise, no change
    uint32_t last_task = current_task;
    if (tasks[last_task].state == RUNNING) {
        tasks[last_task].state = READY;
    }

    _task_switch(&tasks[last_task].ctx, &ctx_scheduler, 0);
}
