# Store callee-saved registers of current task in a C struct
#
# void _task_store(task_context_t *ctx);
.global _task_store
_task_store:
    sw ra, 0(a0)
    sw sp, 4(a0)
    sw s0, 8(a0)
    sw s1, 12(a0)
    sw s2, 16(a0)
    sw s3, 20(a0)
    sw s4, 24(a0)
    sw s5, 28(a0)
    sw s6, 32(a0)
    sw s7, 36(a0)
    sw s8, 40(a0)
    sw s9, 44(a0)
    sw s10, 48(a0)
    sw s11, 52(a0)

    ret

# Swap two tasks curr and next, in privilege 'mode':
#
# void _task_switch(task_context_t *curr, task_context_t *next, int mode, task_handle_t *next_task);
#
# Stores the state of the current task's registers and loads the next.
.global _task_switch
_task_switch:

    sw ra, 0(a0)
    sw sp, 4(a0)
    sw s0, 8(a0)
    sw s1, 12(a0)
    sw s2, 16(a0)
    sw s3, 20(a0)
    sw s4, 24(a0)
    sw s5, 28(a0)
    sw s6, 32(a0)
    sw s7, 36(a0)
    sw s8, 40(a0)
    sw s9, 44(a0)
    sw s10, 48(a0)
    sw s11, 52(a0)

    # Store the handle of the next task in mscratch
    csrw mscratch, a3

    lw ra, 0(a1)
    lw sp, 4(a1)
    lw s0, 8(a1)
    lw s1, 12(a1)
    lw s2, 16(a1)
    lw s3, 20(a1)
    lw s4, 24(a1)
    lw s5, 28(a1)
    lw s6, 32(a1)
    lw s7, 36(a1)
    lw s8, 40(a1)
    lw s9, 44(a1)
    lw s10, 48(a1)
    lw s11, 52(a1)

# Stay in machine mode for mode = 0
    bnez a2, _to_user
    ret

# Change to user mode for mode != 0
_to_user:
    csrw mepc, ra
    mret
