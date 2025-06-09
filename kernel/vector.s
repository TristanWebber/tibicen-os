# All 31 interrupt entries jump to same label
.balign 0x100
.global _vector_table
_vector_table:
    .option push
    .option norvc
    j _trap_entry
    j _systick_handler
    .rept 30
    j _interrupt_handler
    .endr
    .option pop
    .size _vector_table, .-_vector_table

# Exception handler - always executes in machine mode
# Store caller-saved registers and call C handler
.global _trap_entry
_trap_entry:

    # Get the handle of the running process
    csrr tp, mscratch

    # When tp is set as UINT32_MAX, we're already in the kernel and already
    # using a kernel stack
    bltz tp, _trap_store_regs

    # Store the user stack pointer
    sw sp, 4(tp)

    # Switch to the kernel stack for that process
    lw sp, 0(tp)

_trap_store_regs:
    addi sp, sp, -76

    sw ra,  0(sp)
    sw t0,  4(sp)
    sw t1,  8(sp)
    sw t2, 12(sp)
    sw s0, 16(sp)
    sw s1, 20(sp)
    sw a0, 24(sp)
    sw a1, 28(sp)
    sw a2, 32(sp)
    sw a3, 36(sp)
    sw a4, 40(sp)
    sw a5, 44(sp)
    sw a6, 48(sp)
    sw a7, 52(sp)
    sw t3, 56(sp)
    sw t4, 60(sp)
    sw t5, 64(sp)
    sw t6, 68(sp)

    # Pass pointer to trapframe in a0
    mv a0, sp
    call trap_handler

    # Restore registers
    lw ra,  0(sp)
    lw t0,  4(sp)
    lw t1,  8(sp)
    lw t2, 12(sp)
    lw s0, 16(sp)
    lw s1, 20(sp)
    #lw a0, 24(sp) # skip restoring a0 - will return value for some syscalls
    lw a1, 28(sp)
    lw a2, 32(sp)
    lw a3, 36(sp)
    lw a4, 40(sp)
    lw a5, 44(sp)
    lw a6, 48(sp)
    lw a7, 52(sp)
    lw t3, 56(sp)
    lw t4, 60(sp)
    lw t5, 64(sp)
    lw t6, 68(sp)

    addi sp, sp, 76

    # Get the handle of the running process
    csrr tp, mscratch

    # When tp is set as UINT32_MAX, something has gone wrong
    bltz tp, bad_trap

    # Restore the user stack pointer
    lw sp, 4(tp)

    mret

# Do nothing: Not implemented yet
.global _interrupt_handler
_interrupt_handler:
    # Machine exception program counter stores the calling instruction. Adding
    # one word to the mepc returns to the following instruction
    csrr a0, mepc
    addi a0, a0, 4
    csrw mepc, a0
    mret

.global _systick_handler
_systick_handler:
    # Get the handle of the running process
    csrr tp, mscratch

    # When tp is set as UINT32_MAX, we're already in the kernel and already
    # using a kernel stack
    bltz tp, _systick_store_regs

    # Store the user stack pointer
    sw sp, 4(tp)

    # Switch to the kernel stack for that process
    lw sp, 0(tp)

_systick_store_regs:
    addi sp, sp, -128

    sw ra,  0(sp)
    sw sp,  4(sp)
    sw gp,  8(sp)
    sw tp, 12(sp)
    sw t0, 16(sp)
    sw t1, 20(sp)
    sw t2, 24(sp)
    sw s0, 28(sp)
    sw s1, 32(sp)
    sw a0, 36(sp)
    sw a1, 40(sp)
    sw a2, 44(sp)
    sw a3, 48(sp)
    sw a4, 52(sp)
    sw a5, 56(sp)
    sw a6, 60(sp)
    sw a7, 64(sp)
    sw s2, 68(sp)
    sw s3, 72(sp)
    sw s4, 76(sp)
    sw s5, 80(sp)
    sw s6, 84(sp)
    sw s7, 88(sp)
    sw s8, 92(sp)
    sw s9, 96(sp)
    sw s10, 100(sp)
    sw s11, 104(sp)
    sw t3, 108(sp)
    sw t4, 112(sp)
    sw t5, 116(sp)
    sw t6, 120(sp)

    call systick_handler

    lw ra,  0(sp)
    lw sp,  4(sp)
    lw gp,  8(sp)
    lw tp, 12(sp)
    lw t0, 16(sp)
    lw t1, 20(sp)
    lw t2, 24(sp)
    lw s0, 28(sp)
    lw s1, 32(sp)
    lw a0, 36(sp)
    lw a1, 40(sp)
    lw a2, 44(sp)
    lw a3, 48(sp)
    lw a4, 52(sp)
    lw a5, 56(sp)
    lw a6, 60(sp)
    lw a7, 64(sp)
    lw s2, 68(sp)
    lw s3, 72(sp)
    lw s4, 76(sp)
    lw s5, 80(sp)
    lw s6, 84(sp)
    lw s7, 88(sp)
    lw s8, 92(sp)
    lw s9, 96(sp)
    lw s10, 100(sp)
    lw s11, 104(sp)
    lw t3, 108(sp)
    lw t4, 112(sp)
    lw t5, 116(sp)
    lw t6, 120(sp)

    addi sp, sp, 128

    # Get the handle of the running process
    csrr tp, mscratch

    # When tp is set as UINT32_MAX, no change
    bltz tp, _systick_handler_exit

    # Restore the user stack pointer
    lw sp, 4(tp)

_systick_handler_exit:
    mret
