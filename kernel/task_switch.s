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
    sw gp, 56(a0)
    sw tp, 60(a0)
    sw t0, 64(a0)
    sw t1, 68(a0)
    sw t2, 72(a0)
    sw a0, 76(a0)
    sw a1, 80(a0)
    sw a2, 84(a0)
    sw a3, 88(a0)
    sw a4, 92(a0)
    sw a5, 96(a0)
    sw a6, 100(a0)
    sw a7, 104(a0)
    sw t3, 108(a0)
    sw t4, 112(a0)
    sw t5, 116(a0)
    sw t6, 120(a0)

    # Read the mepc value and store
    csrr t0, mepc
    sw t0, 124(a0)
    lw t0, 64(a0)

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
    sw gp, 56(a0)
    sw tp, 60(a0)
    sw t0, 64(a0)
    sw t1, 68(a0)
    sw t2, 72(a0)
    sw a0, 76(a0)
    sw a1, 80(a0)
    sw a2, 84(a0)
    sw a3, 88(a0)
    sw a4, 92(a0)
    sw a5, 96(a0)
    sw a6, 100(a0)
    sw a7, 104(a0)
    sw t3, 108(a0)
    sw t4, 112(a0)
    sw t5, 116(a0)
    sw t6, 120(a0)

    # Read the mepc value and store
    csrr t0, mepc
    sw t0, 124(a0)

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
    lw gp, 56(a1)
    lw tp, 60(a1)
    #skip t0. Using to restore mepc
    lw t1, 68(a1)
    lw t2, 72(a1)
    lw a3, 88(a1)
    lw a4, 92(a1)
    lw a5, 96(a1)
    lw a6, 100(a1)
    lw a7, 104(a1)
    lw t3, 108(a1)
    lw t4, 112(a1)
    lw t5, 116(a1)
    lw t6, 120(a1)

    # Restore the mepc
    lw t0, 124(a1)
    csrw mepc, t0
    lw t0, 64(a1)

# Omit registers used for arguments
# Stay in machine mode for mode = 0
    bnez a2, _to_user
    lw a0, 76(a1)
    lw a2, 84(a1)
    lw a1, 80(a1)
    ret

# Change to user mode for mode != 0
_to_user:
    csrw mepc, ra
    lw a0, 76(a1)
    lw a2, 84(a1)
    lw a1, 80(a1)
    mret
