# Write n bytes from char *b to stdout.
#
# int sys_write(char *b, int n);
.global sys_write
sys_write:
    li a7, 1
    ecall
    ret

# Create a task
#
# bool sys_task_create(void *task_function);
.global sys_task_create
sys_task_create:
    li a7, 2
    ecall
    ret

# Delete a task
#
# void sys_task_delete(void);
.global sys_task_delete
sys_task_delete:
    li a7, 3
    ecall
    ret

# Yield to the scheduler
#
# void sys_task_yield(void);
.global sys_task_yield
sys_task_yield:
    li a7, 4
    ecall
    ret

# Put task to sleep for delay_us and yield to scheduler
#
# void sys_task_delay(uint64_t delay_us);
.global sys_task_delay
sys_task_delay:
    li a7, 5
    ecall
    ret
