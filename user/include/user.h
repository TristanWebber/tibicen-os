#ifndef USER_H
#define USER_H

#include <stdint.h>

// Syscalls
void sys_write(char *bytes_to_send, int len);

void sys_task_create(void *task_function);

void sys_task_delete(void);

void sys_task_yield(void);

void sys_task_delay(uint64_t delay_us);

#endif /* USER_H */
