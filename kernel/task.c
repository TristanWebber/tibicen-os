#include "task.h"
#include "riscv.h"
#include "timer.h"

#include <kstdio.h>

#include <stdbool.h>
#include <stdint.h>

#define MAX_TASK_COUNT 4
#define TASK_STACK_WORDS 256 // 1024 bytes = 256 4-byte words

uint32_t task_count = 0;
uint32_t current_task = 0;

// Store os and scheduler states
task_context_t ctx_os;
task_context_t ctx_scheduler;

// User created tasks (future: linked list)
task_handle_t tasks[MAX_TASK_COUNT];

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
        kputs("Tasks: ERROR no free task slot found.");
    }

    // Clear stacks
    for (unsigned int i = 0; i < TASK_STACK_WORDS; i++) {
        task_stacks[free_idx][i] = 0;
        ppstack[free_idx][i] = 0;
    }

    // Set a sentinel value
    task_stacks[free_idx][0] = 0xCAFEF00D;
    ppstack[free_idx][0] = 0xCAFEF00D;

    task_context_t new_task;
    new_task.ra = (uint32_t)task;
    new_task.sp = (uint32_t)&task_stacks[free_idx][TASK_STACK_WORDS - 1];

    tasks[free_idx].ctx = new_task;
    tasks[free_idx].task_function = task;
    tasks[free_idx].state = NEW;
    tasks[free_idx].delay_to_us = 0;
    tasks[free_idx].kernel_sp = (uint32_t)&ppstack[free_idx][TASK_STACK_WORDS - 1];
    tasks[free_idx].user_sp = new_task.sp;

    task_count++;
    return true;
}

void tasks_scheduler(void) {

    // Fire the first task if it has been initialised
    kputs("Tasks: Starting the tasks scheduler.");
    _task_switch(&ctx_scheduler, &tasks[0].ctx, 2, &tasks[0]);

    while(task_count) {
        timer_feed_wdt();
        current_task = (current_task + 1) % MAX_TASK_COUNT;
        task_state_t current_task_state = tasks[current_task].state;

        switch(current_task_state) {
            case NEW:
                tasks[current_task].state = RUNNING;
                _task_switch(&ctx_scheduler, &tasks[current_task].ctx, 2, &tasks[current_task]);
                break;
            case READY:
                tasks[current_task].state = RUNNING;
                _task_switch(&ctx_scheduler, &tasks[current_task].ctx, 0, &tasks[current_task]);
                break;
            case PENDING:
                // Switch tasks where timer has expired
                if (timer_uptime_us() >= tasks[current_task].delay_to_us) {
                    tasks[current_task].state = RUNNING;
                    tasks[current_task].delay_to_us = 0;
                    _task_switch(&ctx_scheduler, &tasks[current_task].ctx, 0, &tasks[current_task]);
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
    _task_switch(&ctx_scheduler, &ctx_os, 0, (task_handle_t *)UINT32_MAX);
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

    _task_switch(&tasks[last_task].ctx, &ctx_scheduler, 0, (task_handle_t *)UINT32_MAX);
}

// Clear task stacks before first use
void task_clear_stacks(void) {
    for (unsigned int task = 0; task < MAX_TASK_COUNT; task++) {
        for (unsigned int word = 0; word < TASK_STACK_WORDS; word++) {
            task_stacks[task][word] = 0;
            ppstack[task][word] = 0;
        }
    }
}
