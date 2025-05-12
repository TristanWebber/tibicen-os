# All 31 interrupt entries jump to same label
.balign 0x100
.global _vector_table
_vector_table:
    .option push
    .option norvc
    j _trap_entry
    .rept 31
    j _interrupt_handler
    .endr
    .option pop
    .size _vector_table, .-_vector_table

# Store caller-saved registers and call C handler
.global _trap_entry
_trap_entry:
    # TODO: This continues to use the caller's stack. Switch to callee stack.
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
